#include "lap.h"

Lap::Lap()
{

}

QTime Lap::getLapTime()
{
    QTime result = timeEnd;
    long msec = ((timeInit.hour() * 3600 + timeInit.minute() * 60 + timeInit.second()) * 1000) + timeInit.msec();
    return result.addMSecs(-msec);
}
