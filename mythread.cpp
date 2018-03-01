#include "mythread.h"

MyThread::MyThread(int ID, QObject *parent) :
    QThread(parent)
{
    this->socketDescriptor = ID;
}

void MyThread::run()
{
    // thread starts here
    //qDebug() << socketDescriptor << " Starting thread";
    socket = new QTcpSocket();
    if(!socket->setSocketDescriptor(this->socketDescriptor))
    {
        emit error(socket->error());
        return;
    }

    //qDebug() << "inicia";
    connect(socket, SIGNAL(readyRead()), this, SLOT(readyRead()),Qt::DirectConnection);
    connect(socket, SIGNAL(disconnected()), this, SLOT(disconnected()),Qt::DirectConnection);

    //qDebug() << socketDescriptor << " Client connected";

    // make this thread a loop
    exec();
}

void MyThread::readyRead()
{
    QByteArray Data = socket->readAll();

    //qDebug() << socketDescriptor << " Data in: " << Data;
    //TODO: send data to application
    emit tagReceived(Data);

    socket->write(Data);
}

void MyThread::disconnected()
{
    //qDebug() << socketDescriptor << " Disconnected";
    socket->close();
    socket->deleteLater();
    //emit finish();
    exit(0);
    //qDebug() << "destruye";
}
