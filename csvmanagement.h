#ifndef CSVMANAGEMENT_H
#define CSVMANAGEMENT_H

#include <QFile>
#include <QList>
#include <QTextStream>

#include "referee.h"

class CSVManagement
{
public:
    CSVManagement();

    void createReport(QList<Referee*> referees);
};

#endif // CSVMANAGEMENT_H
