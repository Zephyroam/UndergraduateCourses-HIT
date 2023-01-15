#pragma once
#include "heads.h"

// 读取数据
char *read_data(char *file_name)
{
    char *buffer = new char[DATA_SIZE];
    int p = 0;
    char c;
    ZeroMemory(buffer, sizeof(buffer));
    FILE *file = fopen(file_name, "r");

    while((c = fgetc(file)) != EOF)
        buffer[p++] = c;
    
    fclose(file);
    return buffer;
}

// 生成存储文件名
char *get_save_file_name()
{
    char *save_file_name = new char[256];
    strcpy(save_file_name, "received_file_");
    char timestr[256];
    time_t t = time(0);
    strftime(timestr, 256, "%H%M%S", localtime(&t));
    strcat(save_file_name, timestr);
    strcat(save_file_name, ".txt");
    return save_file_name;
}

// 存储数据
void save_data(char *file_name, char *data)
{
    FILE *file = fopen(file_name, "a");
    fwrite(data, 1, strlen(data), file);
    fclose(file);
}

// WSA初始化
ERROR_CODE Initialize_WSA(SOCKET *s, SOCKADDR_IN *addrServer, bool server)
{
    srand((unsigned)time(NULL));
    // Init WSA
    WORD sockVersion = MAKEWORD(2, 2);
    WSADATA wsaData;
    if (WSAStartup(sockVersion, &wsaData) != 0)
    {
        printf("Init WSA Error!\n");
        return INIT_FAILED;
    }

    // Create a new socket
    (*s) = socket(AF_INET, SOCK_DGRAM, 0);
    if ((*s) == INVALID_SOCKET)
    {
        printf("Create a new socket Error!\n");
        return INIT_FAILED;
    }

    if (server)
    {
        // 设置套接字为非阻塞模式
        int iMode = 1; // 1：非阻塞，0：阻塞
        ioctlsocket((*s), FIONBIO, (u_long FAR *)&iMode); // 非阻塞设置
    }

    (*addrServer).sin_family = AF_INET;
    (*addrServer).sin_port = htons(SERVER_PORT);
    if (server)
        (*addrServer).sin_addr.S_un.S_addr = htonl(INADDR_ANY);
    else
        (*addrServer).sin_addr.S_un.S_addr = inet_addr(SERVER_IP);
    return INIT_SUCCEEDED;
}

void printTips(char *protocol)
{
    printf("*****************************************\n");
    printf("   1. Test %s as server\n", protocol);
    printf("   2. Test %s as client\n", protocol);
    printf("   3. Quit\n");
    printf("*****************************************\n");
}

char *ParseCode(uint8_t code)
{
    if (code == 0)
        return (char *)"REQUEST_SUCCEEDED";
    else if (code == 1)
        return (char *)"REQUEST_FAILED";
    else if (code == 3)
        return (char *)"INIT_SUCCEEDED";
    else if (code == 4)
        return (char *)"STAGE_FINISHED";
    else if (code == 5)
        return (char *)"STAGE_ERROR";
    else if (code == 100)
        return (char *)"BEGIN_TEST_INFO";
    else if (code == 101)
        return (char *)"EXIT_TEST_INFO";
    return (char *)"ERROR";
}

bool lossInLossRatio(float lossRatio)
{
    int lossBound = (int)(lossRatio * 100);
    int r = rand() % 101;
    if (r <= lossBound)
        return true;
    return false;
}

void ProcessError(ERROR_CODE ec)
{
    printf("## An error occurred: %s\n", ParseCode(ec));
}