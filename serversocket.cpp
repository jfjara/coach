#include "serversocket.h"
#include <QMutex>

//ServerSocket::ServerSocket(DorsalesTagsDialog* dorsalesDialog, QObject* parent) : QTcpServer(parent)
//{
//    this->dorsalesDialog = dorsalesDialog;
//}

ServerSocket::ServerSocket(int tipo, MyCoachMainWindow* app, DorsalesTagsDialog* dialog, CalibradorDialog* calibrador, QObject* parent) : QTcpServer(parent)
{
    //server = new QTcpServer();
    this->app  = app;
    this->tipo = tipo;
    this->dorsalesDialog = dialog;
    this->calibrador = calibrador;
    //connect(server, SIGNAL(newConnection()), this, SLOT(incomingConnection()));
}

void ServerSocket::StartServer()
{
    if(!this->listen(QHostAddress::Any, 3456))
    {
        qDebug() << "Could not start server";
    }
    else
    {
        qDebug() << "Listening...";
    }
}

void ServerSocket::tagReceivedBySocket(QString tag)
{
    tag = tag.replace(" ", "");
    QStringList split = tag.split("#");
    tag = split[0];
    QString timestamp = split[1];
    //qDebug() << "TS: " << timestamp;
    //if (DataManagement::getInstance()->addTagToProcess(tag)) {
    if (tipo == 1) {
        app->receiveTag(tag);
    } else if (tipo == 2) {
        dorsalesDialog->setTag(tag);
    } else if (tipo == 3) {
        calibrador->recibeDato();
    }
    //}

}

void ServerSocket::incomingConnection(int socketDescriptor)
{
    //qDebug() << socketDescriptor << " Connecting...";
    MyThread *thread = new MyThread(socketDescriptor, this);
    //connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));
    connect(thread, SIGNAL(tagReceived(QString)), this, SLOT(tagReceivedBySocket(QString)));
    thread->start();
}
