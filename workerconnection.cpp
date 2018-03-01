#include "workerconnection.h"

WorkerConnection::WorkerConnection(QObject* parent)
{

    timer = new QTimer(this);
    timer->setSingleShot(true);
    timer->start(1000);
    connect(timer, SIGNAL(timeout()), this, SLOT(doWork()));

    QThread* thread = new QThread(parent);
    this->moveToThread(thread);
    thread->start();
}

void WorkerConnection::doWork()
{
    ClientSocket client;
    client.start("192.168.0.157", 11000);
    timer->start(1000);
    //QThread* thisthread = this->thread();
    //QThread* mainthread = QCoreApplication::instance()->thread();
    //delete mythingy;
}
