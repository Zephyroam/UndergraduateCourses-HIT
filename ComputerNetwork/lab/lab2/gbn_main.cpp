#include "gbn_server.cpp"
#include "gbn_client.cpp"

#pragma comment(lib, "ws2_32.lib")

int main()
{
    while (true)
    {
        printTips((char *)"GBN");
        int choice;
        scanf("%d", &choice);
        if (choice == 1)
        {
            // server
            GBN_Server *server = new GBN_Server();
            server->start();
            delete server;
        }
        else if (choice == 2)
        {
            // client
            GBN_Client *client = new GBN_Client();
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