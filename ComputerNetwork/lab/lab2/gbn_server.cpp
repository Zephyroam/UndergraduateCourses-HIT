#include "heads.h"
#include "utils.cpp"

// GBN服务器端
class GBN_Server
{
private:
    bool ack[SEQ_SIZE];
    int sentSeq = -1; //be about to send
    int ackedSeq = -1;
    int totalSeq;

    char buffer[BUFFER_SIZE];

    // 能否继续发送
    bool seqIsAvailable()
    {
        if (totalSeq - 1 == sentSeq)
            return false;
        if (sentSeq - ackedSeq >= SEND_WIND_SIZE)
            return false;
        return ack[(sentSeq + 1) % SEQ_SIZE] || sentSeq == -1;
    }

    // 超时处理
    void timeoutHandler()
    {
        printf("Timeout Error\n");
        // all data of current window need to be resend
        for (int i = ackedSeq + 1; i <= sentSeq; ++i)
            ack[i % SEQ_SIZE] = true;
        sentSeq = ackedSeq;
    }

    // Ack处理
    void ackHandler(char c)
    {
        int ackCounter = 0;
        unsigned char idx = (unsigned char)c;
        printf("Receive an ack of %d\n", idx);
        if ((ackedSeq + 1) % SEQ_SIZE <= idx)
        {
            for (int i = (ackedSeq + 1) % SEQ_SIZE; i <= idx; ++i, ++ackCounter)
                ack[i] = true;
        }
        else
        {
            for (int i = (ackedSeq + 1) % SEQ_SIZE; i <= idx + SEQ_SIZE; ++i, ++ackCounter)
                ack[i % SEQ_SIZE] = true;
        }
        ackedSeq += ackCounter % SEQ_SIZE;
    }

    // 握手阶段
    ERROR_CODE ShakeHandsStage(SOCKET sockServer, SOCKADDR_IN addrClient)
    {
        int waitCount = 0;
        printf("Begin testing GBN protocol, please don't abort the process\n");
        printf("Shake hands stage\n");
        Sleep(100);
        while (true)
        {
            ZeroMemory(buffer, sizeof(buffer));
            int recvSize = recvfrom(sockServer, buffer, BUFFER_SIZE, 0, ((SOCKADDR *)&addrClient), &SOCKADDR_Length);
            if (recvSize < 0)
            {
                if (++waitCount > TIMEOUT_COUNT)
                {
                    printf("Timeout error\n");
                    return STAGE_ERROR;
                }
                Sleep(500);
            }
            else if ((unsigned char)buffer[0] == GBN_CONFORM_INFO)
            {
                printf("Begin transfering file using GBN\n");
                return STAGE_FINISHED;
            }
            else if ((unsigned char)buffer[0] == GBN_REQUEST_INFO)
            {
                printf("Receive request information from client\n");
                ZeroMemory(buffer, sizeof(buffer));
                buffer[0] = GBN_RESPONSE_INFO;
                sendto(sockServer, buffer, strlen(buffer) + 1, 0, (SOCKADDR *)&addrClient, sizeof(SOCKADDR));
            }
            else
            {
                printf("Receive a wrong status code\n");
                return STAGE_ERROR;
            }
        }
    }

    // 挥手阶段
    ERROR_CODE FinishTransferStage(SOCKET sockServer, SOCKADDR_IN addrClient)
    {
        int waitCount = TIMEOUT_COUNT;
        int resendTime = 0;
        printf("Tansfer finished, sending information\n");
        Sleep(100);
        while (true)
        {
            ZeroMemory(buffer, sizeof(buffer));
            int recvSize = recvfrom(sockServer, buffer, BUFFER_SIZE, 0, ((SOCKADDR *)&addrClient), &SOCKADDR_Length);
            if (recvSize < 0)
            {
                if (++waitCount > TIMEOUT_COUNT)
                {
                    ZeroMemory(buffer, sizeof(buffer));
                    buffer[0] = TANSFER_FINISHED_INFO;
                    sendto(sockServer, buffer, strlen(buffer) + 1, 0, (SOCKADDR *)&addrClient, sizeof(SOCKADDR));
                    printf("Timeout, resending\n");
                    waitCount = 0;
                    if (++resendTime > TIMEOUT_COUNT)
                        return STAGE_ERROR;
                }
                Sleep(500);
            }
            else if ((unsigned char)buffer[0] == TRANSFER_FINISHED_ACK_INFO)
            {
                printf("Tansfer end\n");
                return STAGE_FINISHED;
            }
            else
            {
                printf("Receive a wrong status code\n");
                return STAGE_ERROR;
            }
        }
    }

    // 数据传输阶段
    ERROR_CODE TransferDataStage(SOCKET sockServer, SOCKADDR_IN addrClient, char *data)
    {
        int waitCount = 0;
        while (true)
        {
            if (seqIsAvailable())
            {
                ZeroMemory(buffer, sizeof(buffer));
                buffer[0] = (++sentSeq) % SEQ_SIZE;
                ack[sentSeq % SEQ_SIZE] = false;
                
                if (sentSeq == totalSeq - 1)
                    memcpy(buffer + 1, data + PACK_SIZE * sentSeq, strlen(data + PACK_SIZE * sentSeq));
                else
                    memcpy(buffer + 1, data + PACK_SIZE * sentSeq, PACK_SIZE);

                printf("send a packet with a seq of %d\n", sentSeq % SEQ_SIZE);
                sendto(sockServer, buffer, BUFFER_SIZE, 0, (SOCKADDR *)&addrClient, sizeof(SOCKADDR));
                Sleep(100);
            }
            // Waiting for ack
            ZeroMemory(buffer, sizeof(buffer));
            int recvSize = recvfrom(sockServer, buffer, BUFFER_SIZE, 0, ((SOCKADDR *)&addrClient), &SOCKADDR_Length);
            if (recvSize < 0)
            {
                if (++waitCount > TIMEOUT_COUNT)
                {
                    // resend packet
                    timeoutHandler();
                    waitCount = 0;
                }
            }
            else
            {
                // accept ack
                ackHandler(buffer[0]);
                waitCount = 0;
                if (ackedSeq == totalSeq - 1)
                {
                    printf("Transfer finished\n");
                    return STAGE_FINISHED;
                }
            }
            Sleep(200);
        }
    }

    // 初始化
    ERROR_CODE Initialize(SOCKET *s)
    {
        SOCKET sockServer;
        SOCKADDR_IN addrServer;
        Initialize_WSA(&sockServer, &addrServer, true);

        if (bind(sockServer, (SOCKADDR *)&addrServer, sizeof(addrServer)) == SOCKET_ERROR)
        {
            printf("Init listening Error!\n");
            WSACleanup();
            return INIT_FAILED;
        }
        *s = sockServer;

        for (int i = 0; i < SEQ_SIZE; ++i)
            ack[i] = true;

        return INIT_SUCCEEDED;
    }

public:
    ERROR_CODE start()
    {
        ERROR_CODE ec;

        SOCKET sockServer;
        if ((ec = Initialize(&sockServer)) != INIT_SUCCEEDED)
        {
            ProcessError(ec);
            return -1;
        }
        SOCKADDR_IN addrClient;

        char *data = read_data((char *)"./bert.txt");
        totalSeq = strlen(data) / PACK_SIZE + 1;

        printf("There are %d packets\n", totalSeq);

        puts("Waiting for connection");
        while (true)
        {
            ZeroMemory(buffer, sizeof(buffer));

            int recvSize = recvfrom(sockServer, buffer, BUFFER_SIZE, 0, ((SOCKADDR *)&addrClient), &SOCKADDR_Length);
            if (recvSize < 0)
            {
                Sleep(200);
                continue;
            }
            printf("receive from client: %s\n", ParseCode(buffer[0]));
            if ((unsigned char)buffer[0] == BEGIN_TEST_INFO)
            {
                ShakeHandsStage(sockServer, addrClient); // 握手阶段
                TransferDataStage(sockServer, addrClient, data); // 数据传输阶段
                FinishTransferStage(sockServer, addrClient); // 挥手阶段
                break;
            }
            Sleep(200);
        }
        closesocket(sockServer);
        WSACleanup();
        return REQUEST_SUCCEEDED;
    }
};
