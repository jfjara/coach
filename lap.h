#ifndef LAP_H
#define LAP_H

#include <QTime>

class Lap
{
public:
    Lap();

    QTime timeInit;
    QTime timeEnd;

    QTime getLapTime();
};

#endif // LAP_H
