#include "datamanagement.h"

DataManagement* DataManagement::instance = 0;

DataManagement::DataManagement()
{

}

DataManagement* DataManagement::getInstance()
{
    if (instance == 0)
    {
       instance = new DataManagement();
       //mock
       instance->mockBonus();
    }
    return instance;
}

Referee* DataManagement::getRefereeByDorsal(int dorsal)
{
    for (Referee* referee : refereesMap.values()) {
        if (referee->dorsal == dorsal) {
            return referee;
        }
    }
    return Q_NULLPTR;
}

double DataManagement::calcularBonificacion(QString categoria, SPORT_TEST prueba, QTime tiempo)
{
    QList<Bonus*> bonus = bonusMap.value(categoria).value(prueba);

    bool encontrado = false;
    for (Bonus* b : bonus) {
        double msecsInit, secsInit, msecsEnd, secsEnd;
        msecsInit = modf (b->timeInit , &secsInit);
        msecsEnd = modf (b->timeInit , &secsEnd);
        QTime init = QTime(0,0);
        QTime end = QTime(0,0);
        init.addSecs(secsInit);
        init.addMSecs(msecsInit);
        end.addSecs(secsEnd);
        end.addMSecs(msecsEnd);

        if (tiempo >= init && tiempo <= end) {
            return b->points;
        }

    }

    return 0.0;
}

bool DataManagement::addTagToProcess(QString tag)
{
    bool result = false;
    mutex.lock();
    if (!tagsProcesando.contains(tag)) {
        qDebug() << "add " << tag;
        tagsProcesando.append(tag);
        result = true;
    }
    mutex.unlock();
    return result;
}

void DataManagement::deleteTagToProcess(QString tag)
{
    mutex.lock();
    int pos = -1;
    for (int i = 0; i < DataManagement::getInstance()->tagsProcesando.size(); i++) {
        if (tag == DataManagement::getInstance()->tagsProcesando.at(i)) {
            pos = i;
            break;
        }
    }
    if (pos > -1) {
        DataManagement::getInstance()->tagsProcesando.removeAt(pos);
    }
    mutex.unlock();
}

QString DataManagement::popTag()
{
    QString result = "";
    mutex.lock();
    if (tagsProcesando.size() > 0) {
        result = tagsProcesando.at(0);
        tagsProcesando.removeFirst();
    }
    mutex.unlock();
    return result;
}

SPORT_TEST DataManagement::getTipoPrueba(QString texto)
{
    if (texto.simplified().toUpper() == "CARRERA DE 6X40 METROS") {
        return C6X40;
    } else if (texto.simplified().toUpper() == "2000 MTS") {
        return C2000MTS;
    } else if (texto.simplified().toUpper() == "PRUEBA DE CAMPO") {
        return PRUEBA_DE_CAMPO;
    }
    return NINGUNA_PRUEBA;
}

CATEGORIAS DataManagement::getTipoCategoria(QString texto)
{
    if (texto.simplified().toUpper() == "OFICIALES") {
        return OFICIALES;
    }
    return NINGUNA_CATEGORIA;
}

void DataManagement::fillBonus(Bonus* b, QTime t1, QTime t2, double p)
{
    //b->timeInit = t1;
    //b->timeEnd = t2;
    //b->points = p;
}

void DataManagement::mockBonus()
{

}

QTime DataManagement::getTopePC(QString categoria, QString sexo)
{
    double secsDouble = 0.0;
    if (sexo == "MASCULINO") {
        secsDouble = bonusMap.value(categoria).value(PRUEBA_DE_CAMPO).last()->timeEnd;
    } else {
        secsDouble = bonusFemMap.value(categoria).value(PRUEBA_DE_CAMPO).last()->timeEnd;
    }
    int secs = (int)secsDouble;

    int h = secs / 3600;
    int m = ( secs % 3600 ) / 60;
    int s = ( secs % 3600 ) % 60;

    int ms = 0;
    if (s > 0) {
        ms = ((secsDouble - ((double)s + ((double)m * 60.0))) * 1000) + 1;
    }

    QTime t(h, m, s, ms);
    qDebug() << "Tope: " << t.toString("hh:mm:ss.zzz");
    return t;
}

QTime DataManagement::getTope2000(QString categoria, QString sexo)
{
    double time = 0.0;
    if (sexo == "MASCULINO") {
        time = bonusMap.value(categoria).value(C2000MTS).last()->timeEnd;
    } else {
        time = bonusFemMap.value(categoria).value(C2000MTS).last()->timeEnd;
    }
    int minutos = floor(time);
    double secsDouble = (time - minutos) * 100;

    QTime t(0, minutos, (int)secsDouble, 0);
    return t;
}

QTime DataManagement::getTope6x40(QString categoria, QString sexo)
{
    double secsDouble = 0.0;
    if (sexo == "MASCULINO") {
        secsDouble = bonusMap.value(categoria).value(C6X40).last()->timeEnd;
    } else {
        secsDouble = bonusFemMap.value(categoria).value(C6X40).last()->timeEnd;
    }
    int secs = (int)secsDouble;

    int h = secs / 3600;
    int m = ( secs % 3600 ) / 60;
    int s = ( secs % 3600 ) % 60;

    int ms = ((secsDouble - (double)s) * 1000) + 1;

    QTime t(h, m, s, ms);
    return t;
}

void DataManagement::createBonusCategory(SPORT_TEST type)
{
//    QList<Bonus*> listaBonus;
//    bonusMap.insert(type, listaBonus);
}

void DataManagement::insertBonus(SPORT_TEST type, Bonus* bonus)
{
//    QList<Bonus*> bonusList = bonusMap.value(type);
//    bonusList.push_back(bonus);
//    bonusMap.insert(type, bonusList);
}

void DataManagement::addBonus(QString categoria, SPORT_TEST type, Bonus* bonus)
{

    if (bonusMap.contains(categoria)) {
        QMap<SPORT_TEST, QList<Bonus*>> map = bonusMap.value(categoria);
        if (map.contains(type)) {
            QList<Bonus*> bonusList = map.value(type);
            bonusList.push_back(bonus);
            map.insert(type, bonusList);
            bonusMap.insert(categoria, map);
            //insertBonus(type, bonus);
        } else {
            QList<Bonus*> listaBonus;
            listaBonus.push_back(bonus);
            map.insert(type, listaBonus);
            bonusMap.insert(categoria, map);
            //createBonusCategory(type);
            //insertBonus(type, bonus);
        }
    } else {
        QMap<SPORT_TEST, QList<Bonus*>> map;
        QList<Bonus*> listaBonus;
        listaBonus.push_back(bonus);
        map.insert(type, listaBonus);
        bonusMap.insert(categoria, map);
    }
}

void DataManagement::addBonusFem(QString categoria, SPORT_TEST type, Bonus* bonus)
{

    if (bonusFemMap.contains(categoria)) {
        QMap<SPORT_TEST, QList<Bonus*>> map = bonusFemMap.value(categoria);
        if (map.contains(type)) {
            QList<Bonus*> bonusList = map.value(type);
            bonusList.push_back(bonus);
            map.insert(type, bonusList);
            bonusFemMap.insert(categoria, map);
            //insertBonus(type, bonus);
        } else {
            QList<Bonus*> listaBonus;
            listaBonus.push_back(bonus);
            map.insert(type, listaBonus);
            bonusFemMap.insert(categoria, map);
            //createBonusCategory(type);
            //insertBonus(type, bonus);
        }
    } else {
        QMap<SPORT_TEST, QList<Bonus*>> map;
        QList<Bonus*> listaBonus;
        listaBonus.push_back(bonus);
        map.insert(type, listaBonus);
        bonusFemMap.insert(categoria, map);
    }
}

double DataManagement::getBonificacion(QString categoria, QString sexo, SPORT_TEST tipo, double mxs)
{
    if (sexo.trimmed().toUpper() == "MASCULINO") {

        if (bonusMap.contains(categoria)) {

            QList<Bonus*> bonus = bonusMap.value(categoria).value(tipo);

            int i = 0;
            while (i < bonus.size()) {
                if (bonus.at(i)->timeEnd >= mxs && bonus.at(i)->timeInit <= mxs) {
                    return bonus.at(i)->points;
                }
                i++;
            }
        }
    } else {
        if (bonusFemMap.contains(categoria)) {

            QList<Bonus*> bonus = bonusFemMap.value(categoria).value(tipo);

            int i = 0;
            while (i < bonus.size()) {
                if (bonus.at(i)->timeEnd >= mxs && bonus.at(i)->timeInit <= mxs) {
                    return bonus.at(i)->points;
                }
                i++;
            }
        }
    }
    return 0.0;

}

double DataManagement::getBonificacion(QString categoria, QString sexo, SPORT_TEST tipo, int msecs)
{

    if (bonusMap.contains(categoria)) {

        int secs = msecs / 1000;
        double ms = (double)(msecs - secs * 1000) / 1000.0;

        double time = (double)secs + ms;

        QList<Bonus*> bonus = bonusMap.value(categoria).value(tipo);

        int i = 0;
        while (i < bonus.size()) {
            if (bonus.at(i)->timeEnd >= time && bonus.at(i)->timeInit <= time) {
                return bonus.at(i)->points;
            }
            i++;
        }
    }
    return 0.0;
}

void DataManagement::addReferee(Referee* referee, QString tag)
{
    //referee->id = getNewIdForReferee();
    //referees.push_back(referee);
    referee->tag = tag;
    refereesMap.insert(tag, referee);
}

void DataManagement::removeReferee(QString tag)
{
    //referees.removeAt(position);
    refereesMap.remove(tag);
}

int DataManagement::getNewIdForReferee()
{
    /*int result = 0;
    for (int i = 0; i < referees.size(); i++) {
        if (result < referees.at(0)->id) {
            result = referees.at(0)->id;
        }
    }*/
    //return result + 1;
    return 0;
}

void DataManagement::orderRefereeIds()
{

}


