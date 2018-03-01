#ifndef CONNECTIONTHREAD_H
#define CONNECTIONTHREAD_H

#include <QObject>
#include <QThread>
#include <QTcpSocket>
#include <QHostAddress>

class ConnectionThread : public QThread
{
    Q_OBJECT
public:    
    explicit ConnectionThread(int iID, QObject *parent = 0);
    virtual ~ConnectionThread() {}
    void run();

    bool hasError = false;

public slots:
    void readyRead();
    void disconnected();
    void tcpErrorHandler();
    //void checkConnection();

private:
    QTcpSocket *socket = 0;
    int id;

};

#endif // CONNECTIONTHREAD_H
