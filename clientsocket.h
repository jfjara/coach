#ifndef CLIENTSOCKET_H
#define CLIENTSOCKET_H

#include <QObject>
#include <QTcpSocket>
#include <QHostAddress>

class ClientSocket: public QObject
{
    Q_OBJECT
public:
    ClientSocket();
    ~ClientSocket();
     void start(QString address, quint16 port);

     void connectDevice();
     void initReader();
     void stopReader();

public slots:
    void connectedToServer();


private:
  QTcpSocket client;

  bool isConnected = false;
};

#endif // CLIENTSOCKET_H
