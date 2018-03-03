#ifndef EXCELREADER_H
#define EXCELREADER_H

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

    void readBonus(QString categoria, QString pathFile);
    void readReferees(QString pathFile);
    void createResultsReport(QString path, QList<ResultadoArbitro> resultados);

private:

    QString findTagInMap(int dorsal, QMap<int, QString> map);

};

#endif // EXCELREADER_H
