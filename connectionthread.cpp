#include "connectionthread.h"

ConnectionThread::ConnectionThread(int ID, QObject *parent) :
    QThread(parent)
{
    id = ID;
}

void ConnectionThread::run()
{
    QHostAddress addr("192.168.0.143");
    //while (1) {
    //    this->sleep(10);
        if (socket != 0) {
            socket->deleteLater();
            disconnect(socket);
            delete socket;
            socket = 0;
        }
        socket = new QTcpSocket();
        connect(socket, SIGNAL(connected()), this, SLOT(readyRead()));
        connect(socket, SIGNAL(error(QAbstractSocket::SocketError)),
                    this, SLOT(tcpErrorHandler()));
        socket->connectToHost(addr, 11000);
    //}
    exec();
}

void ConnectionThread::tcpErrorHandler()
{
    socket->deleteLater();
    disconnect(socket);
    delete socket;
    socket = 0;
    hasError = true;
}

void ConnectionThread::readyRead()
{
   socket->deleteLater();
   disconnect(socket);
   delete socket;
   socket = 0;
   hasError = false;
}

void ConnectionThread::disconnected()
{
    //qDebug() << socketDescriptor << " Disconnected";
    socket->deleteLater();
    exit(0);
}
