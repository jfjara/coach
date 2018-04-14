#ifndef EXCELREADER_H
#define EXCELREADER_H

#include <QApplication>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QList>
#include <QMap>
#include "bonus.h"
#include <qaxobject.h>
#include "constantes.h"
#include "datamanagement.h"
#include "sessionmanagement.h"
#include "resultadoarbitro.h"

class ExcelReader
{
public:
    ExcelReader();

    void readBonus(QString categoria, QString sexo, QString pathFile);
    void readReferees(QString pathFile);
    void createResultsReport(QString path, QList<ResultadoArbitro*> resultados, QString categoria, bool noAptos);
    void create6x40Report(QString path, QList<ResultadoArbitro*> resultados, QString categoria);
    void create5x40Report(QString path, QList<ResultadoArbitro*> resultados, QString categoria);
    void create2000Report(QString path, QList<ResultadoArbitro*> resultados, QString categoria);
    void createPCReport(QString path, QList<ResultadoArbitro*> resultados, QString categoria);

    QString getTitulo(QString categoria);

private:

    QString findTagInMap(int dorsal, QMap<int, QString> map);

};

#endif // EXCELREADER_H
