#include "server.h"

UINT Server::ReciveClientMessageThread(LPVOID param)
{
    ServerParam* serverParam = (ServerParam*)param;

    SOCKET socket = serverParam->socket;
    Server* server = serverParam->server;

    while (1)
    {
        if (server->ReciveClientMessage(socket))
        {
            break;
        }
    }
    return 0;
}

UINT Server::ListenNewClientsThread(LPVOID param)
{
    Server* server = (Server*)param;

    while (1)
    {
        server->ListenNewClients();
    }

    return 0;
}

Server::Server()
{
    isConnected = false;

    WSADATA wsaData;

    sockaddr_in local;

    int wsaret = WSAStartup(0x101, &wsaData);

    if (wsaret != 0)
    {
        return;
    }

    local.sin_family = AF_INET;
    local.sin_addr.s_addr = INADDR_ANY;
    local.sin_port = htons((u_short)8084);

    listenClient = socket(AF_INET, SOCK_STREAM, 0);


    if (listenClient == INVALID_SOCKET)
    {
        return;
    }

    if (bind(listenClient, (sockaddr*)&local, sizeof(local)) != 0)
    {
        return;
    }


    if (listen(listenClient, 10) != 0)
    {
        return;
    }

    isConnected = true;

    return;
}

void Server::StartListenThread()
{
    AfxBeginThread(ListenNewClientsThread, this);
}

Server::~Server()
{
    closesocket(listenClient);

    WSACleanup();
}

void Server::ListenNewClients()
{
    sockaddr_in from;
    int fromlen = sizeof(from);

    client = accept(listenClient, (struct sockaddr*)&from, &fromlen);

    if (client != INVALID_SOCKET)
    {
        clientList.push_back(client);
    }

    ServerParam* serverParam = new ServerParam;
    serverParam->server = this;
    serverParam->socket = client;

    AfxBeginThread(ReciveClientMessageThread, serverParam);
}

bool Server::IsConnected()
{
    return isConnected;
}

int Server::ServerSendMessagePort(string message)
{
    message = "server: " + message;

    return this->SendMessagePort(message, NULL);
}

int Server::SendMessagePort(string message, SOCKET from)
{
    int stat = 0;
    list<SOCKET>::iterator iterator;

    if (clientList.size() == 0)
    {
        return 0;
    }

    for (iterator = clientList.begin(); iterator != clientList.end(); iterator++)
    {
        if (from == *iterator)
        {
            continue;
        }

        stat = send(*iterator, message.c_str(), message.size() + 1, 0);

        if (stat == -1)
        {
            clientList.remove(*iterator);
        }
    }

    if (stat == -1)
    {
        return 1;
    }

    return 0;
}

int Server::ReciveClientMessage(SOCKET socket)
{
    char tempBuffer[4096];
    int stat;

    stat = recv(socket, tempBuffer, 4096, 0);
    if (stat == -1)
    {
        clientList.remove(socket);
        return 1;
    }
    else
    {
        cout << tempBuffer << "\n";
        SendMessagePort(tempBuffer, socket);
        return 0;
    }
    return 0;

}
