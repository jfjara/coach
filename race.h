#ifndef RACE_H
#define RACE_H

#include <QList>
#include <QTime>

#include "referee.h"

class Race
{
public:
    Race();

    QList<Referee> runners;


    void nextLap(Referee* referee, QTime time);

};

#endif // RACE_H
