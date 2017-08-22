
#include <Afxwin.h>
#include <stdio.h>
#include <winsock2.h>
#include <conio.h> 
#include "Ws2tcpip.h"
#include <string>
#include <iostream>

using namespace std;


class Client
{
public:
    Client();
    ~Client();

    void Init(string ip, int port, string userName);
    void StartReceiveMessage();
    int SendMessagePort(string message);
    int ReceiveMessage();
    bool IsConnected();

private:
    bool isConnected;
    int serverPort;

    string serverIPAddress;
    string userName;

    SOCKET conn; 

    static UINT ReceiveMessageThread(LPVOID param);
};
