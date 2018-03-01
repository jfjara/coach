#include "referee.h"
#include <QDebug>

Referee::Referee()
{
    lastRegister = QTime::currentTime();
    lastRegister = lastRegister.addSecs(-120);
}

void Referee::startRace()
{
    chronometre.restart();
}

bool Referee::isInTime(QTime maxTime)
{
    if (laps.last()->timeInit <= maxTime) {
        return true;
    }
    return false;
}

QString Referee::getTimeLastLap()
{
    QTime result = QTime(0, 0);

    if (laps.size() > 1) {
        QTime end = laps.at(laps.size() - 1)->timeInit;
        QTime init = laps.at(laps.size() - 2)->timeInit;
        int msecs = init.msecsTo(end);
        result = result.addMSecs(msecs);
    }
    return result.toString("hh:mm:ss.zzz");

}

void Referee::addLap40x6(QTime time, QTime maxTime)
{
    if (finished) {
        return;
    }

    if (isStartLap) {
        chronometre.start();
        isStartLap = false;
        lastRegister = QTime::currentTime();
    } else {
         isStartLap = true;
         registerLap(time);

         if (!isInTime(maxTime)) {
             if (!nuevaOportunidad) {
                 nuevaOportunidad = true;
             } else {
                 estaDescalificado = true;
                 finished = true;
             }
         }

         chronometre.stop();
         if (!nuevaOportunidad) {
             if (laps.size() == totalLapsRace) {
                 finished = true;
             }
         } else {
             if (laps.size() == totalLapsRace + 1) {
                 finished = true;
             }
         }
    }
}

void Referee::addLap(QTime time)
{
    if (finished) {
        return;
    }

    if (laps.size() == 0) {
        chronometre.start();
        registerLap(QTime(0,0));
    } else {
        registerLap(time);
    }
    if (laps.size() - 1 >= totalLapsRace) {
        finished = true;
        chronometre.stop();
        isStartLap =true;
    }
}

QString Referee::getTotalTime()
{
    QTime result = QTime(0,0);

    if (laps.size() > 0) {
        result = laps.at(0)->timeInit;
        long msec = 0;
        for (int i = 1; i < laps.size(); i++) {
            QTime timeInit = laps.at(i)->timeInit;
            msec = ((timeInit.hour() * 3600 + timeInit.minute() * 60 + timeInit.second()) * 1000) + timeInit.msec();
            result = result.addMSecs(msec);
        }
    }
    return result.toString("hh:mm:ss.zzz");
}


bool Referee::isAvailableToRegister(int seconds)
{
    QTime now = QTime::currentTime();

   // qDebug() << "Secs: " << QString::number(lastRegister.secsTo(now));

    if (lastRegister.secsTo(now) >= seconds) {
        return true;
    }
    return false;
}

void Referee::registerLap(QTime time)
{
    Lap* lap = new Lap();
    lap->timeInit = time;
    laps.push_back(lap);
    lastRegister = QTime::currentTime();
}

void Referee::clean()
{
    while (laps.size() > 0) {
        laps.removeLast();
    }
    chronometre.stop();
    lastRegister = QTime::currentTime();
    lastRegister = lastRegister.addSecs(-120);
}

