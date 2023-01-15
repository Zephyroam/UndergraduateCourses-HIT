#include "sr_server.cpp"
#include "sr_client.cpp"

#pragma comment(lib, "ws2_32.lib")

int main()
{
    while (true)
    {
        printTips((char *)"SR");
        int choice;
        scanf("%d", &choice);
        if (choice == 1)
        {
            // server
            SR_Server *server = new SR_Server();
            server->start();
            delete server;
        }
        else if (choice == 2)
        {
            // client
            SR_Client *client = new SR_Client();
            client->start();
            delete client;
        }
        else if (choice == 3)
            break;
        else
            printf("Invalid choice!\n");
    }
    return 0;
}