#ifndef MYTHREAD_H
#define MYTHREAD_H

#include <QThread>
#include <QTcpSocket>
#include <QDebug>

class MyThread: public QThread
{
    Q_OBJECT
public:
    explicit MyThread(int iID, QObject *parent = 0);
    void run();

signals:
    void error(QTcpSocket::SocketError socketerror);
    void tagReceived(QString tag);
    void finish();

public slots:
    void readyRead();
    void disconnected();

private:
    QTcpSocket *socket;
    int socketDescriptor;

};


#endif // MYTHREAD_H
