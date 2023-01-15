#pragma once
#include <stdio.h>
#include <winsock2.h>
#include <stdint.h>
#include <string.h>
#include <string>
#include <stdlib.h>
#include <time.h>
#include <cstdio>
#include <fstream>
#include <iostream>

#define REQUEST_SUCCEEDED 0 // 请求成功
#define REQUEST_FAILED 1 // 请求失败
#define INIT_FAILED 2 // 初始化失败
#define INIT_SUCCEEDED 3 // 初始化成功
#define STAGE_FINISHED 4 // 阶段完成
#define STAGE_ERROR 5 // 阶段错误
#define BUFFER_SIZE 1026 //缓冲区大小
#define SERVER_PORT 12340 // port
#define TIMEOUT_COUNT 10 //超时计数
#define SEND_WIND_SIZE 10 //发送窗口大小
#define RECV_WIND_SIZE 10 //接收窗口大小
#define PACK_SIZE 1024 //包大小
#define SEQ_SIZE 20 //序列号大小
#define DATA_SIZE (PACK_SIZE * 113)
#define SERVER_IP "127.0.0.1"

#define BEGIN_TEST_INFO 100 //测试开始
#define EXIT_TEST_INFO 101 //测试结束
#define GBN_REQUEST_INFO 200 //GBN请求
#define GBN_RESPONSE_INFO 201 //GBN响应
#define GBN_CONFORM_INFO 202 //GBN确认
#define TANSFER_FINISHED_INFO 203 //传输完成
#define TRANSFER_FINISHED_ACK_INFO 204 //传输完成确认

#define SENT 10 //已发送
#define ACKED 11 //已确认
#define NEEDTORESEND 12 //需要重传

float packetLossRatio = 0.2; //丢包率
float ackLossRatio = 0.2; //ACK丢包率

typedef uint8_t ERROR_CODE;

int SOCKADDR_Length = sizeof(SOCKADDR);