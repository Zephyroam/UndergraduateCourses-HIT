#include "heads.h"
#include "utils.cpp"

class SR_Client // SR客户端
{
private:
    char buffer[BUFFER_SIZE];
    char receive_buffer[SEQ_SIZE][BUFFER_SIZE];
    bool received[SEQ_SIZE];
    int WinL = 0; // 接收窗口左端
    int WinR = -1; // 接收窗口右端
    char *save_file_name;

    // 判断当前序列号是否在接收窗口中
    bool isValidSeq(int seq)
    {
        for (int i = WinL; i <= WinR; i++)
            if (i % SEQ_SIZE == seq)
                return true;
        for (int i = WinR + 1; i <= WinL + RECV_WIND_SIZE - 1; i++)
            if (i % SEQ_SIZE == seq)
            {
                for (int j = WinR + 1; j < i; j++)
                    received[j % SEQ_SIZE] = false;
                WinR = i;
                return true;
            }
        return false;
    }

    // 握手阶段
    ERROR_CODE ShakeHandsStage(SOCKET socketClient, SOCKADDR_IN addrServer)
    {
        ZeroMemory(buffer, sizeof(buffer));
        buffer[0] = GBN_REQUEST_INFO;
        sendto(socketClient, buffer, strlen(buffer) + 1, 0, (SOCKADDR *)&addrServer, sizeof(SOCKADDR));
        while (true)
        {
            ZeroMemory(buffer, sizeof(buffer));
            recvfrom(socketClient, buffer, BUFFER_SIZE, 0, (SOCKADDR *)&addrServer, &SOCKADDR_Length);
            if ((unsigned char)buffer[0] == GBN_RESPONSE_INFO)
            {
                printf("Receive response information from server\n");
                printf("Ready for file transmission\n");
                ZeroMemory(buffer, sizeof(buffer));
                buffer[0] = GBN_CONFORM_INFO;
                sendto(socketClient, buffer, strlen(buffer) + 1, 0, (SOCKADDR *)&addrServer, sizeof(SOCKADDR));
                return STAGE_FINISHED;
            }
            Sleep(500);
        }
        return STAGE_ERROR;
    }

    // 数据传输阶段
    ERROR_CODE TransferDataStage(SOCKET socketClient, SOCKADDR_IN addrServer)
    {
        char data[BUFFER_SIZE];
        ZeroMemory(buffer, sizeof(buffer));
        while (true)
        {
            ZeroMemory(data, sizeof(data));
            int recvSize = recvfrom(socketClient, buffer, BUFFER_SIZE, 0, (SOCKADDR *)&addrServer, &SOCKADDR_Length);

            if (recvSize < 0)
            {
                continue;
                Sleep(200);
            }

            if ((unsigned char)buffer[0] == TANSFER_FINISHED_INFO)
            {
                buffer[0] = TRANSFER_FINISHED_ACK_INFO;
                sendto(socketClient, buffer, strlen(buffer) + 1, 0, (SOCKADDR *)&addrServer, sizeof(SOCKADDR));
                printf("Receive transfer finished information from server\n");
                return STAGE_FINISHED;
            }

            int seq = buffer[0];
            strcpy(data, buffer + 1);
            ZeroMemory(buffer, sizeof(buffer));

            if (lossInLossRatio(packetLossRatio))
            {
                printf("The packet with a seq of %d loss\n", seq);
                continue;
            }
            printf("receive a packet with a seq of %d\n", seq);

            buffer[0] = seq;
            if (isValidSeq(seq))
            {
                received[seq] = true;
                ZeroMemory(receive_buffer[seq], BUFFER_SIZE);
                strcpy(receive_buffer[seq], data);
                while (received[WinL % SEQ_SIZE] && WinL <= WinR)
                {
                    // 存储数据
                    save_data(save_file_name, receive_buffer[WinL % SEQ_SIZE]);
                    WinL++;
                }
            }
            if (lossInLossRatio(packetLossRatio))
            {
                printf("The ack of %d loss\n", buffer[0]);
                continue;
            }
            sendto(socketClient, buffer, strlen(buffer) + 1, 0, (SOCKADDR *)&addrServer, sizeof(SOCKADDR));
            printf("send a ack of %d\n", buffer[0]);
            Sleep(500);
        }
        return STAGE_ERROR;
    }

public:
    ERROR_CODE start()
    {
        SOCKET socketClient;
        SOCKADDR_IN addrServer;
        Initialize_WSA(&socketClient, &addrServer, false);
        save_file_name = get_save_file_name();

        for (int i = 0; i < SEQ_SIZE; i++)
            received[i] = true;

        ZeroMemory(buffer, sizeof(buffer));
        buffer[0] = BEGIN_TEST_INFO;
        sendto(socketClient, buffer, strlen(buffer) + 1, 0, (SOCKADDR *)&addrServer, sizeof(SOCKADDR));
        printf("Begin testing GBN protocol\n");
        printf("The loss ratio of packet is %.2f, the loss ratio of ack is %.2f\n", packetLossRatio, ackLossRatio);
        ShakeHandsStage(socketClient, addrServer); // 握手阶段
        TransferDataStage(socketClient, addrServer); // 数据传输阶段

        closesocket(socketClient);
        WSACleanup();
        return REQUEST_SUCCEEDED;
    }
};