#include "sessionmanagement.h"

SessionManagement::SessionManagement()
{

}

QMap<int, QString> SessionManagement::loadAsignacionesDorsales()
{
    QFile file("dorsales.info");
    QMap<int, QString> map;
    if(!file.open(QIODevice::ReadOnly)) {
        return map;
        //QMessageBox::information(0, "No se encuentra el fichero de dorsales", file.errorString());
    }

    QTextStream in(&file);

    while(!in.atEnd()) {
        QString line = in.readLine();
        QStringList fields = line.split(";");
        map.insert(QString(fields.at(0)).toInt(), fields[1]);
    }

    file.close();
    return map;
}


QMap<int, QList<QTime>> SessionManagement::loadPrueba(QString filename)
{
    QMap<int, QList<QTime>> map;
   // QString filename = path + "\\6x40.ses";
    QFile file(filename);
    if(!file.open(QIODevice::ReadOnly)) {
        return map;
    }

    QTextStream in(&file);
    while(!in.atEnd())
    {
        QString line = in.readLine();
        QStringList fields = line.split("#");

        QString primero = fields.at(0);
        int dorsal = primero.split("=")[1].toInt();

        QList<QTime> times;
        for (int i = 1; i < fields.size(); i++) {
            QString time = fields.at(i);
            times.append(QTime::fromString(time, "hh:mm:ss.zzz"));
        }
        qSort(times.begin(), times.end(), SessionManagement::timeLessThan);
        map.insert(dorsal, times);
    }
    return map;
}

void SessionManagement::saveAsignaciones(QMap<int, QString> map)
{
    QFile file("dorsales.info");
    file.remove();
    if ( file.open(QIODevice::ReadWrite))
    {
        QTextStream stream( &file );
        for (int dorsal : map.keys()) {
            QString tag = map.value(dorsal);
            stream << QString::number(dorsal) << ";" << tag << '\n';
        }
    }
}

void SessionManagement::save(QString path, int tipoPrueba)
{
    QString filename = "";
    switch (tipoPrueba) {
    case 0:
        filename = path + "\\6x40.ses";
        break;
    case 1:
        filename = path + "\\2000.ses";
        break;
    case 2:
        filename = path + "\\pc.ses";
        break;
    case 3:
        filename = path + "\\5x40.ses";
        break;

    }

    QFile file( filename );
    if ( file.open(QIODevice::ReadWrite))
    {
        QTextStream stream( &file );

        //stream << "P=" + QString::number(tipoPrueba) << "#";

        for (Referee* referee : DataManagement::getInstance()->refereesMap.values()) {

            stream << "D=" << QString::number(referee->dorsal) << "#";
            for (int i = 0; i < referee->laps.size(); i++) {
                stream << referee->laps[i]->timeInit.toString("hh:mm:ss.zzz");
                if (i + 1 < referee->laps.size()) {
                    stream << "#";
                }
            }
            stream << ";" << '\n';
        }
    }

}
