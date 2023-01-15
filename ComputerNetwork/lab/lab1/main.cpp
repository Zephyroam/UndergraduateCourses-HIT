#include "server.h"
#include "server.cpp" 

#pragma comment(lib, "ws2_32.lib")

int main()
{
    // running on port 26357
    Server server(26357);
    server.start();
    return 0;
}
