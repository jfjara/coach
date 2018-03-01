#ifndef SERVERSOCKET_H
#define SERVERSOCKET_H

class MyCoachMainWindow;
class DorsalesTagsDialog;
class CalibradorDialog;

#include <QObject>
#include <QTcpServer>
#include "mythread.h"
#include "mycoachmainwindow.h"
#include "datamanagement.h"
#include "dorsalestagsdialog.h"
#include "calibradordialog.h"

class ServerSocket:  public QTcpServer
{
    Q_OBJECT
public:
   // explicit ServerSocket(DorsalesTagsDialog* app, QObject* parent=0);
    explicit ServerSocket(int tipo, MyCoachMainWindow* app, DorsalesTagsDialog* dialog, CalibradorDialog* calibrador, QObject* parent=0);
    ~ServerSocket() { this->close(); }

    //virtual ~ServerSocket() { delete server; }

    void StartServer();

    MyCoachMainWindow* app;
    DorsalesTagsDialog* dorsalesDialog;
    CalibradorDialog* calibrador;
    int tipo;


public slots:
    void tagReceivedBySocket(QString tag);

protected:
    void incomingConnection(int socketDescriptor);
};

#endif // SERVERSOCKET_H
