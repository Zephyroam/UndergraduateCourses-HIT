#include "heads.h"
#include "utils.cpp"

class SR_Server
{
private:
    ERROR_CODE ack[SEQ_SIZE];
    int waitCount[SEQ_SIZE];
    int winR = -1;
    int winL = 0;
    int totalSeq;

    char buffer[BUFFER_SIZE];

    // 获取下一个需要发送的序列号
    int getNextSeq()
    {
        for (int i = winL; i <= winR; ++i)
            if (ack[i % SEQ_SIZE] == NEEDTORESEND) // 查看有没有需要重传的
                return i;
        if (totalSeq - 1 == winR || winR - winL + 1 >= SEND_WIND_SIZE)
            return -2;
        if (ack[(winR + 1) % SEQ_SIZE] == ACKED || winR == -1)
            return ++winR;
        else
            return -2;
    }

    // 处理计时器
    void timerHandler()
    {
        for (int i = winL; i <= winR; ++i)
        {
            if (ack[i % SEQ_SIZE] != SENT)
                continue;
            waitCount[i % SEQ_SIZE]++;
            // resend packet
            if (waitCount[i % SEQ_SIZE] > TIMEOUT_COUNT)
            {
                printf("%d Timeout Error\n", i % SEQ_SIZE);
                waitCount[i % SEQ_SIZE] = 0;
                ack[i % SEQ_SIZE] = NEEDTORESEND;
            }
        }
    }

    // 处理Ack
    void ackHandler(char c)
    {
        int ackCounter = 0;
        unsigned char idx = (unsigned char)c;
        printf("Receive an ack of %d\n", idx);
        ack[idx] = ACKED;
        waitCount[idx] = 0;
        while (ack[winL % SEQ_SIZE] == ACKED && winL <= winR)
            winL++;
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
        while (true)
        {
            ZeroMemory(buffer, sizeof(buffer));
            int NextSeq = getNextSeq();
            if (NextSeq != -2)
            {
                buffer[0] = NextSeq % SEQ_SIZE;
                ack[NextSeq % SEQ_SIZE] = SENT;
                if (NextSeq == totalSeq - 1)
                    memcpy(buffer + 1, data + PACK_SIZE * NextSeq, strlen(data + PACK_SIZE * NextSeq));
                else
                    memcpy(buffer + 1, data + PACK_SIZE * NextSeq, PACK_SIZE);
                
                printf("send a packet with a seq of %d\n", NextSeq % SEQ_SIZE);

                sendto(sockServer, buffer, BUFFER_SIZE, 0, (SOCKADDR *)&addrClient, sizeof(SOCKADDR));
                Sleep(100);
            }
            // Waiting for ack
            ZeroMemory(buffer, sizeof(buffer));
            int recvSize = recvfrom(sockServer, buffer, BUFFER_SIZE, 0, ((SOCKADDR *)&addrClient), &SOCKADDR_Length);
            if (recvSize < 0)
                timerHandler();
            else
            {
                // accept ack
                ackHandler(buffer[0]);
                if (winL == totalSeq)
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
        {
            ack[i] = ACKED;
            waitCount[i] = 0;
        }

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
