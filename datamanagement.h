#ifndef DATAMANAGEMENT_H
#define DATAMANAGEMENT_H

#include <QList>
#include <QMap>
#include <QModelIndex>
#include <QMutex>
#include <QDebug>
#include "referee.h"
#include "bonus.h"

enum SPORT_TEST {
    C6X40, C2000MTS, PRUEBA_DE_CAMPO, NINGUNA_PRUEBA
};

enum CATEGORIAS {
    OFICIALES, NINGUNA_CATEGORIA
};

class DataManagement
{
public:

    static DataManagement* getInstance();

    void addBonusFem(QString categoria, SPORT_TEST type, Bonus* bonus);
    void addBonus(QString categoria, SPORT_TEST type, Bonus* bonus);
    void addReferee(Referee* referee, QString tag);
    void removeReferee(QString tag);
    SPORT_TEST getTipoPrueba(QString texto);
    CATEGORIAS getTipoCategoria(QString texto);

    double getBonificacion(QString categoria, QString sexo, SPORT_TEST tipo, double mxs);
    double getBonificacion(QString categoria, QString sexo, SPORT_TEST tipo, int msecs);

    double calcularBonificacion(QString categoria, SPORT_TEST prueba, QTime tiempo);

    bool addTagToProcess(QString tag);
    void deleteTagToProcess(QString tag);
    QString popTag();

    QTime getTope6x40(QString categoria);

    QMap<QString, Referee*> refereesMap;
    QMap<QString, QMap<SPORT_TEST, QList<Bonus*>>> bonusMap;

    QMap<QString, QMap<SPORT_TEST, QList<Bonus*>>> bonusFemMap;

    QList<QString> tagsProcesando;

    int totalLaps = 1;
    int distanceLap = 40;

    int secondsBetweenRegister = 4;

    QMutex mutex;

private:
    static DataManagement* instance;
    DataManagement();

    int getNewIdForReferee();
    void orderRefereeIds();
    void createBonusCategory(SPORT_TEST type);
    void insertBonus(SPORT_TEST type, Bonus* bonus);




    void mockBonus();
    void fillBonus(Bonus* b, QTime t1, QTime t2, double p);
};

#endif // DATAMANAGEMENT_H
