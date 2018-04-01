#ifndef LAP_H
#define LAP_H

#include <QTime>

class Lap
{
public:
    Lap();

    QTime timeInit;
    QTime timeEnd = QTime(0,0);

    QTime getLapTime();
    //bool isFinished();

    bool finalizada = false;
    bool descalificada = false;
    bool fueraTiempo = false;
};

#endif // LAP_H
