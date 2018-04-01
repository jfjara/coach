#ifndef SESSIONMANAGEMENT_H
#define SESSIONMANAGEMENT_H

#include "datamanagement.h"
#include "referee.h"
#include <QFile>
#include <QTextStream>
#include <QMap>
#include <QtAlgorithms>

class SessionManagement
{
public:
    SessionManagement();

    QMap<int, QString> loadAsignacionesDorsales();
    void save(QString filename, int tipoPrueba);
    void deleteFile(QString filename, int tipoPrueba);
    void saveAsignaciones(QMap<int, QString> map);

    static bool timeLessThan(const QTime &t1, const QTime &t2) {
         return t1 < t2;
    }

    QMap<int, QList<QTime>> loadPrueba(QString path);
};

#endif // SESSIONMANAGEMENT_H
