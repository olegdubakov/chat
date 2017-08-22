#include "client.h"

bool EnterString(string message, string &resultString)
{
    cout << message << endl;

    getline(cin, resultString);

    return resultString.size() == 0;
}

void ShowMessage(string message)
{
    cout << message << endl;
    getch();
}

int main(int argc, char* argv[])
{
    Client client;

    string buffer;
    int port = 8084;

    string serverAddress;
    string userName;

    if (EnterString("Set server IP: ", serverAddress))
    {
        ShowMessage("Server IP not set!");
        return 0;
    }

    if (EnterString("Set your name: ", userName))
    {
        ShowMessage("Your name not set!");
        return 0;
    }

    client.Init(serverAddress.c_str(), port, userName);

    if (!client.IsConnected())
    {
        ShowMessage("Unable to connect to the IP");
        return 0;
    }

    ShowMessage("----------\nConnected\n----------");

    client.StartReceiveMessage();

    while (true)
    {
        getline(cin, buffer);

        if (buffer.length() == 0)
        {
            break;
        }

        if (client.SendMessagePort(buffer))
        {
            cout << endl << "Server error" << endl;
            break;
        }
    }

    return 0;
}

