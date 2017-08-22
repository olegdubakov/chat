#include "client.h"

Client::Client()
{
    this->isConnected = false;
}

bool Client::IsConnected()
{
    return this->isConnected;
}

void Client::Init(string ip, int port, string userName)
{
    this->userName = userName;
    this->serverIPAddress = ip;
    this->serverPort = port;

    struct hostent *hp;
    unsigned int addr;
    struct sockaddr_in server;

    WSADATA wsaData;

    int wsaret = WSAStartup(0x101, &wsaData);

    if (wsaret != 0)
    {
        return;
    }

    this->conn = socket(AF_INET, SOCK_STREAM, 0);

    if (this->conn == INVALID_SOCKET)
    {
        return;
    }

    addr = inet_addr(this->serverIPAddress.c_str());
    hp = gethostbyaddr((char*)&addr, sizeof(addr), AF_INET);

    if (hp == NULL)
    {
        closesocket(this->conn);
        return;
    }

    server.sin_addr.s_addr = *((unsigned long*)hp->h_addr);
    server.sin_family = AF_INET;
    server.sin_port = htons(this->serverPort);

    if (connect(this->conn, (struct sockaddr*)&server, sizeof(server)))
    {
        closesocket(this->conn);
        return;
    }

    this->isConnected = true;

    return;
}

Client::~Client()
{
    if (this->isConnected)
    {
        closesocket(this->conn);
    }
}

int Client::SendMessagePort(string message)
{
    message = this->userName + ": " + message;

    int iStat = 0;

    iStat = send(this->conn, message.c_str(), message.size() + 1, 0);

    if (iStat == -1)
    {
        return 1;
    }

    return 0;
}

int Client::ReceiveMessage()
{
    char data[4096];
    int result = 0;

    result = recv(this->conn, data, 4096, 0);
    if (result == -1)
    {
        return 1;
    }

    cout << data << endl;

    return 0;
}

UINT Client::ReceiveMessageThread(LPVOID param)
{
    Client *client = (Client*)param;

    while (1)
    {
        if (client->ReceiveMessage())
        {
            break;
        }
    }

    return 0;
}

void Client::StartReceiveMessage()
{
    AfxBeginThread(ReceiveMessageThread, this);
}