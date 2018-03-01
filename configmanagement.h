#ifndef CONFIGMANAGEMENT_H
#define CONFIGMANAGEMENT_H

#include <QList>
#include <QStringList>
#include <QTextStream>
#include "referee.h"

class ConfigManagement
{
public:
    ConfigManagement();

    static QList<Referee*> loadReferees();
    static void saveReferees(QList<Referee*> referees);
};

#endif // CONFIGMANAGEMENT_H
