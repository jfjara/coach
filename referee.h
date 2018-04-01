#ifndef REFEREE_H
#define REFEREE_H

#include <QString>
#include <QList>
#include <QTime>
#include <QDateTime>
#include "chrono.h"


#include "lap.h"



class Referee
{
public:
    Referee();

    int dorsal;
    QString name;

    QDateTime born;
    QString comarca;
    QString gender;
    QString categoria;

    QString dni;
    int age;
    //Category category = OFICIALES;

    /*RACE**/
    bool finished = false;
    int totalLapsRace = 0;
    QString tag;
    Chrono* chronometre = Q_NULLPTR;
    QTime lastRegister;
    QList<Lap*> laps;
    bool estaDescalificado = false;
    bool nuevaOportunidad = false;
    bool pintadaNuevaOportunidad = false;

    bool isStartLap = true;

    /***MOCK****/
    double speed;

    void addLap40x6(QTime time, QTime maxTime);
    void addLap(QTime time);
    void addLapPC(QTime time);
    void startRace();
    void registerLap(QTime time);
    void clean();
    bool isAvailableToRegister(int seconds);
    QString getTotalTime();
    QString getTimeLastLap();    
    bool checkFinished(int numeroVueltasPrueba) {
        if (laps.size() == numeroVueltasPrueba && !nuevaOportunidad) {
            finished = true;
            return finished;
        }
        if (laps.size() >= numeroVueltasPrueba + 1) {
            finished = true;
            return finished;
        }
        return finished;
    }

    void checkFueraTiempo(QTime maxTime) {
        if (!isInTime(maxTime)) {
            if (!nuevaOportunidad) {
                nuevaOportunidad = true;
            } else {
                estaDescalificado = true;
                finished = true;
            }
        }
    }

    bool isInTime(QTime maxTime);
};

#endif // REFEREE_H
