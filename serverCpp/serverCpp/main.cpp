#include "server.h"


void ShowMessage(string message)
{
    cout << message << endl;
    _getch();
}

int main(int argc, char* argv[])
{
    Server server;
    string buffer;

    if (!server.IsConnected())
    {
        ShowMessage("Failed to initialise");
        return 1;
    }

    server.StartListenThread();

    ShowMessage("ServerStarted");

    while (true)
    {
        getline(cin, buffer);

        if (buffer.length() == 0)
        {
            break;
        }
        
        if (server.ServerSendMessagePort(buffer))
        {
            ShowMessage("Problem in connecting to server. Check whether server is running");
            break;
        }
    }

    ShowMessage("End");

    return 0;
}

