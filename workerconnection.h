#ifndef WORKERCONNECTION_H
#define WORKERCONNECTION_H

#include <QObject>
#include <QTimer>
#include <QThread>
#include "clientsocket.h"


class WorkerConnection : QObject
{
    Q_OBJECT
public:
    WorkerConnection(QObject* parent = 0);

    QTimer* timer;

signals:
    void deleteObject(QObject* thingy);
private slots:
    void doWork();
};

#endif // WORKERCONNECTION_H
