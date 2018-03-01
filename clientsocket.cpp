#include "clientsocket.h"

ClientSocket::ClientSocket()
{    
    connect(&client, SIGNAL(connected()), this, SLOT(connectedToServer()));
}

ClientSocket::~ClientSocket()
{
    client.close();
}

void ClientSocket::start(QString address, quint16 port)
{
    QHostAddress addr(address);
    client.connectToHost(addr, port);
    //connectDevice();
}

void ClientSocket::connectedToServer()
{
    isConnected = true;
}

void ClientSocket::connectDevice()
{
    if (!isConnected) {
        return;
    }
    client.write("#CONNECT#<EOF>");
}

void ClientSocket::stopReader()
{
    if (!isConnected) {
        return;
    }
    client.write("#STOP#<EOF>");
}

void ClientSocket::initReader()
{
    if (!isConnected) {
        return;
    }
    client.write("#INIT#<EOF>");
}


