#pragma once
#include <stdio.h>
#include <winsock2.h>
#include <stdint.h>
#include <string.h>
#include <string>

#define REQUEST_SUCCEEDED 0
#define REQUEST_FAILED 1
#define NO_RETURN_INFO 2
#define CHANGE_SUCCEEDED 3
#define CHANGE_FAILED 4
#define HOST_BLOCKED 5
#define USER_BLOCKED 6
#define BUFFER_MAXSIZE 65507
#define HTTP_PORT 80
#define DATE_LENGTH 50
#define CACHE_MAXSIZE 1000
#define DISABLED_MAXSIZE 20

typedef uint8_t ERROR_CODE;

using namespace std;

// HTTP头结构
struct HttpHeader
{
    char method[4];
    char url[1024];
    char host[1024];
    char cookie[1024 * 10];
    HttpHeader()
    {
        ZeroMemory(this, sizeof(HttpHeader));
    }
};

typedef HttpHeader * HttpHeaderP;

// 缓存存放结构
class WebCache
{
public:
    static WebCache *FindCache(HttpHeaderP htp);
    HttpHeaderP htP;
    char buffer[BUFFER_MAXSIZE];
    char date[DATE_LENGTH];
    int buffer_length;
    WebCache()
    {
        ZeroMemory(this->buffer, BUFFER_MAXSIZE);
        ZeroMemory(this->date, DATE_LENGTH);
        this->htP = new HttpHeader();
    }
};

WebCache *Cache_storage[CACHE_MAXSIZE];
int Cache_storage_length = 0;

typedef WebCache * WebCacheP;

// 网站过滤
char *disabledHost[DISABLED_MAXSIZE] = {(char *)"jwc.hit.edu.cn"};
// char *disabledUser[DISABLED_MAXSIZE] = {(char *)"127.0.0.1"};
// 用户过滤
char *disabledUser[DISABLED_MAXSIZE] = {};
// 网站诱导
char *induceSites[DISABLED_MAXSIZE] = {(char *)"today.hit.edu.cn"};
char *targetSites[DISABLED_MAXSIZE] = {(char *)"jwts.hit.edu.cn"};

class Server
{
public:
    Server(int);
    void start();

private:
    SOCKET slisten;
    void CreateThreadToHandleRequest(SOCKET);
    static ERROR_CODE HandleRequest(const string &, string &);
    static DWORD WINAPI RequestThread(LPVOID);
    static HttpHeaderP ParseHttpHeader(char *, int);
    static DWORD RequestFinished(SOCKET, SOCKET, ERROR_CODE);
    static ERROR_CODE ConnectToServer(SOCKET *, char *);
    static ERROR_CODE RequestUsingCache(WebCacheP, SOCKET, SOCKET, char *, int, HttpHeaderP);
    static ERROR_CODE UpdateCache(WebCacheP, HttpHeaderP, char *, int);
};

ERROR_CODE SiteFilter(char *);
ERROR_CODE UserFilter(in_addr);