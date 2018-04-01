#ifndef CHRONO_H
#define CHRONO_H

#include <QElapsedTimer>
#include <QTime>

class Chrono
{
public:
    Chrono();

    QElapsedTimer* t = NULL;

    void start() {
        t = new QElapsedTimer();
        t->start(); }
    void restart() {
        if (t == Q_NULLPTR) {
            t = new QElapsedTimer();
        }
        t->restart();
    }
    void stop() {
        if (t != Q_NULLPTR) {
            delete t;
            t = Q_NULLPTR;
        }
    //    t.restart();
    }
    QTime getTime() {
        if (t == Q_NULLPTR) {
            return QTime(0,0);
        }
        return QTime(0,0).addMSecs(t->elapsed()); }
};

#endif // CHRONO_H
