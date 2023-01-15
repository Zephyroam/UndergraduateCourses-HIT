#include "heads.h"
#include "utils.cpp"

// BGN客户端
class GBN_Client
{
private:
    char buffer[BUFFER_SIZE];
    char *save_file_name;

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
        int recvSeq = -1; //接收窗口大小为 1，已确认的序列号
        int waitSeq = 0;  //等待的序列号
        ZeroMemory(buffer, sizeof(buffer));
        while (true)
        {
            ZeroMemory(data, sizeof(data));
            recvfrom(socketClient, buffer, BUFFER_SIZE, 0, (SOCKADDR *)&addrServer, &SOCKADDR_Length);

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

            if (waitSeq == seq)
            {
                save_data(save_file_name, data);
                waitSeq = (waitSeq + 1) % SEQ_SIZE;
                // printf("%s\n",&buffer[1]);
                recvSeq = seq;
                buffer[0] = seq;
            }
            else
            {
                if (recvSeq == -1)
                    continue;
                buffer[0] = recvSeq;
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