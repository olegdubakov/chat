#include <Afxwin.h>
#include <stdio.h>
#include <conio.h> 
#include<list>
#include <winsock2.h>
#include <string>
#include <iostream>

using namespace std;

class Server
{
public:
    Server();
    ~Server();

    bool IsConnected();
    void ListenNewClients();
    void StartListenThread();
    int SendMessagePort(string sMessage, SOCKET from);
    int ServerSendMessagePort(string sMessage);
    int ReciveClientMessage(SOCKET sRecSocket);

    static UINT ReciveClientMessageThread(LPVOID param);
    static UINT ListenNewClientsThread(LPVOID param);

    typedef struct ServerParam
    {
        Server* server;
        SOCKET socket;
    } ServerParam;

private:
    // true - connected false - not connected
    bool isConnected;

    // Server port
    int port;

    // All socket connected to client
    list<SOCKET> clientList;

    SOCKET client;

    // socket listening for client calls
    SOCKET listenClient;
};
