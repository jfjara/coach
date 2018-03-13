#include "excelreader.h"
#include <qdebug.h>

ExcelReader::ExcelReader()
{

}

QString ExcelReader::getTitulo(QString categoria)
{
    QString resultado = "";
    if (categoria == "OFICIAL") {
        resultado = "OFICIALES";
    }
    if (categoria == "ASISTENTE2B") {
        resultado = "ASISTENTES 2ªB";
    }
    if (categoria == "3DIVISION") {
        resultado = "3ª DIVISION";
    }
    if (categoria == "ASISTENTE3DIVISION") {
        resultado = "ASISTENTES 3ª DIVISION";
    }
    if (categoria == "DIVISIONHONORSENIOR") {
        resultado = "DIVISION DE HONOR SENIOR";
    }
    if (categoria == "PROVINCIAL") {
        resultado = "PROVINCIALES";
    }
    if (categoria == "NUEVOINGRESO") {
        resultado = "DE NUEVO INGRESO";
    }
    return resultado;
}

void ExcelReader::readReferees(QString pathFile)
{
    QAxObject* excel = new QAxObject( "Excel.Application", 0 );
    QAxObject* workbooks = excel->querySubObject( "Workbooks" );
    workbooks->querySubObject( "Open(const QString&)", pathFile);//"C:\\QtWorkspace\\PLANTILLA ARBITROS OFICIALES.xlsx" );
    QAxObject* workbook = excel->querySubObject("ActiveWorkBook");
    QAxObject* worksheet = workbook->querySubObject("Worksheets(int)", 1); //worksheet number

    QAxObject* usedrange = worksheet->querySubObject("UsedRange");
    QAxObject* rows = usedrange->querySubObject("Rows");
    int intRowStart = PLANTILLA_ARBITROS_FILA_INICIO; //usedrange->property("Row").toInt();
    int intRows = rows->property("Count").toInt();

    SessionManagement session;
    QMap<int, QString> mapDorsalesTags = session.loadAsignacionesDorsales();

    for (int i = intRowStart; i < intRowStart + intRows; i++)
    {
        QAxObject * cellDorsal = excel->querySubObject("Cells(Int, Int)", i,  PLANTILLA_ARBITROS_COL_DORSAL);
        QAxObject * cellNombre = excel->querySubObject("Cells(Int, Int)", i,  PLANTILLA_ARBITROS_COL_NOMBRE);
        QAxObject * cellSexo = excel->querySubObject("Cells(Int, Int)", i,  PLANTILLA_ARBITROS_COL_SEXO);
        QAxObject * cellFNac = excel->querySubObject("Cells(Int, Int)", i,  PLANTILLA_ARBITROS_COL_FECHA_NACIMIENTO);
        QAxObject * cellCategoria = excel->querySubObject("Cells(Int, Int)", i,  PLANTILLA_ARBITROS_COL_CATEGORIA);
        QAxObject * cellComarca = excel->querySubObject("Cells(Int, Int)", i,  PLANTILLA_ARBITROS_COL_COMARCA);

        QVariant cellValueNombre = cellNombre->dynamicCall("value");
        QVariant cellValueSexo = cellSexo->dynamicCall("value");
        QVariant cellValueFNac = cellFNac->dynamicCall("value");
        QVariant cellValueCategoria = cellCategoria->dynamicCall("value");
        QVariant cellValueComarca = cellComarca->dynamicCall("value");
        QVariant cellValueDorsal = cellDorsal->dynamicCall("value");

        if (cellValueNombre.toString().simplified() == "") {
            continue;
        }

        Referee* referee = new  Referee();        
        referee->dorsal = cellValueDorsal.toInt();
        referee->name = cellValueNombre.toString().simplified();
        referee->comarca = cellValueComarca.toString().simplified();
        referee->gender = cellValueSexo.toString().simplified().toUpper();
        referee->categoria = cellValueCategoria.toString().trimmed().toUpper();
        DataManagement::getInstance()->addReferee(referee, findTagInMap(referee->dorsal, mapDorsalesTags));

    }
    workbook->dynamicCall("Close()");
    excel->dynamicCall( "Quit()");
    delete excel;
}

QString ExcelReader::findTagInMap(int dorsal, QMap<int, QString> map)
{
    if (!map.contains(dorsal)) {
        return "";
    }
    return map.value(dorsal);
}

void ExcelReader::create6x40Report(QString path, QList<ResultadoArbitro*> resultados, QString categoria)
{
    QAxObject* excel = new QAxObject( "Excel.Application", 0 );
    QAxObject* workbooks = excel->querySubObject( "Workbooks" );
    workbooks->querySubObject( "Open(const QString&)", "c:\\pruebas\\template6x40.xlsx");
    QAxObject* workbook = excel->querySubObject("ActiveWorkBook");
    QAxObject* worksheet = workbook->querySubObject("Worksheets(int)", 1);
    QAxObject* usedrange = worksheet->querySubObject("UsedRange");
    QAxObject* rows = usedrange->querySubObject("Rows");
    int intRowStart = PLANTILLA_ARBITROS_FILA_INICIO;
    int intRows = rows->property("Count").toInt();

    QAxObject * celdaTitulo = excel->querySubObject("Cells(Int, Int)", 4,  5);
    celdaTitulo->setProperty("Value", "RESULTADOS DE PRUEBAS FÍSICAS 6x40 METROS ÁRBITROS " + getTitulo(categoria));

    QAxObject * celdaFecha = excel->querySubObject("Cells(Int, Int)", 4,  18);
    celdaFecha->setProperty("Value", QDateTime::currentDateTime().toString("dd-MMMM-yy"));

    int fila = 8;
    for (ResultadoArbitro* resultado : resultados) {

        QAxObject * celdaDorsal = excel->querySubObject("Cells(Int, Int)", fila,  PLANTILLA_RESULTADOS_DORSAL);
        celdaDorsal->setProperty("Value", QString::number(resultado->arbitro->dorsal));

        QAxObject * celdaNombre = excel->querySubObject("Cells(Int, Int)", fila,  PLANTILLA_RESULTADOS_NOMBRE);
        celdaNombre->setProperty("Value", resultado->arbitro->name);

        int seconds = 0;
        int mseconds = 0;

        for (int i = 0; i < resultado->lista6x40.size(); i++) {
            QAxObject * celdaC6x40 = excel->querySubObject("Cells(Int, Int)", fila,  PLANTILLA_RESULTADOS_C6x40 + i);
            seconds = resultado->lista6x40.at(i).second();
            mseconds = resultado->lista6x40.at(i).msec();
            celdaC6x40->setProperty("Value", QString::number(seconds) + "," + QString::number(mseconds));
        }
        QAxObject * celdaPromedioC6x40 = excel->querySubObject("Cells(Int, Int)", fila,  PLANTILLA_RESULTADOS_PROMEDIO_C6x40);
        seconds = resultado->promedio.second();
        mseconds = resultado->promedio.msec();
        celdaPromedioC6x40->setProperty("Value", QString::number(seconds) + "," + QString::number(mseconds));

        QAxObject * celdaBonusC6x40 = excel->querySubObject("Cells(Int, Int)", fila,  PLANTILLA_RESULTADOS_BONIFICACION_C6x40);
        celdaBonusC6x40->setProperty("Value", QString::number(resultado->bonificacion6x40));

        fila++;
    }
    workbook->querySubObject("SaveAs (const QString&)", path);
    workbook->dynamicCall("Close()");
    excel->dynamicCall( "Quit()");
    delete excel;
}

void ExcelReader::create2000Report(QString path, QList<ResultadoArbitro*> resultados, QString categoria)
{
    QAxObject* excel = new QAxObject( "Excel.Application", 0 );
    QAxObject* workbooks = excel->querySubObject( "Workbooks" );
    workbooks->querySubObject( "Open(const QString&)", "c:\\pruebas\\template2000.xlsx");
    QAxObject* workbook = excel->querySubObject("ActiveWorkBook");
    QAxObject* worksheet = workbook->querySubObject("Worksheets(int)", 1);
    QAxObject* usedrange = worksheet->querySubObject("UsedRange");
    QAxObject* rows = usedrange->querySubObject("Rows");
    int intRowStart = PLANTILLA_ARBITROS_FILA_INICIO;
    int intRows = rows->property("Count").toInt();

    QAxObject * celdaTitulo = excel->querySubObject("Cells(Int, Int)", 4,  5);
    celdaTitulo->setProperty("Value", "RESULTADOS DE PRUEBAS FÍSICAS 2000 METROS ÁRBITROS " +getTitulo(categoria));

    QAxObject * celdaFecha = excel->querySubObject("Cells(Int, Int)", 4,  18);
    celdaFecha->setProperty("Value", QDateTime::currentDateTime().toString("dd-MMMM-yy"));

    int fila = 8;
    for (ResultadoArbitro* resultado : resultados) {

        QAxObject * celdaDorsal = excel->querySubObject("Cells(Int, Int)", fila,  PLANTILLA_RESULTADOS_DORSAL);
        celdaDorsal->setProperty("Value", QString::number(resultado->arbitro->dorsal));

        QAxObject * celdaNombre = excel->querySubObject("Cells(Int, Int)", fila,  PLANTILLA_RESULTADOS_NOMBRE);
        celdaNombre->setProperty("Value", resultado->arbitro->name);

        int seconds = 0;
        int mseconds = 0;

        QAxObject * celdaC2000 = excel->querySubObject("Cells(Int, Int)", fila,  4);
        celdaC2000->setProperty("Value", QString::number(resultado->getResultado2000()));

        QAxObject * celdaBonusC2000 = excel->querySubObject("Cells(Int, Int)", fila,  5);
        celdaBonusC2000->setProperty("Value", QString::number(resultado->bonificacion2000));


        fila++;
    }
    workbook->querySubObject("SaveAs (const QString&)", path);
    workbook->dynamicCall("Close()");
    excel->dynamicCall( "Quit()");
    delete excel;
}

void ExcelReader::createPCReport(QString path, QList<ResultadoArbitro*> resultados, QString categoria)
{
    QAxObject* excel = new QAxObject( "Excel.Application", 0 );
    QAxObject* workbooks = excel->querySubObject( "Workbooks" );
    workbooks->querySubObject( "Open(const QString&)", "c:\\pruebas\\templatePC.xlsx");
    QAxObject* workbook = excel->querySubObject("ActiveWorkBook");
    QAxObject* worksheet = workbook->querySubObject("Worksheets(int)", 1);
    QAxObject* usedrange = worksheet->querySubObject("UsedRange");
    QAxObject* rows = usedrange->querySubObject("Rows");
    int intRowStart = PLANTILLA_ARBITROS_FILA_INICIO;
    int intRows = rows->property("Count").toInt();

    QAxObject * celdaTitulo = excel->querySubObject("Cells(Int, Int)", 4,  5);
    celdaTitulo->setProperty("Value", "RESULTADOS DE PRUEBAS FÍSICAS PRUEBA DE CAMPO ÁRBITROS " + getTitulo(categoria));

    QAxObject * celdaFecha = excel->querySubObject("Cells(Int, Int)", 4,  18);
    celdaFecha->setProperty("Value", QDateTime::currentDateTime().toString("dd-MMMM-yy"));

    int fila = 8;
    for (ResultadoArbitro* resultado : resultados) {

        QAxObject * celdaDorsal = excel->querySubObject("Cells(Int, Int)", fila,  PLANTILLA_RESULTADOS_DORSAL);
        celdaDorsal->setProperty("Value", QString::number(resultado->arbitro->dorsal));

        QAxObject * celdaNombre = excel->querySubObject("Cells(Int, Int)", fila,  PLANTILLA_RESULTADOS_NOMBRE);
        celdaNombre->setProperty("Value", resultado->arbitro->name);

        int seconds = 0;
        int mseconds = 0;

        QAxObject * celdaC2000 = excel->querySubObject("Cells(Int, Int)", fila,  4);
        celdaC2000->setProperty("Value", QString::number(resultado->getResultado2000()));

        QAxObject * celdaBonusC2000 = excel->querySubObject("Cells(Int, Int)", fila,  5);
        celdaBonusC2000->setProperty("Value", QString::number(resultado->bonificacion2000));

        QString resultadoPC = "";
        if (resultado->resultadoPC.minute() > 0) {
            resultadoPC = QString::number(resultado->resultadoPC.minute()) + ":";
        }
        resultadoPC = resultadoPC + QString::number(resultado->resultadoPC.second()) + "," + QString::number(resultado->resultadoPC.msec());

        QAxObject * celdaCPC = excel->querySubObject("Cells(Int, Int)", fila,  PLANTILLA_RESULTADOS_CPC);
        celdaCPC->setProperty("Value", resultadoPC);

        QAxObject * celdaBonusCPC = excel->querySubObject("Cells(Int, Int)", fila,  PLANTILLA_RESULTADOS_BONIFICACION_CPC);
        celdaBonusCPC->setProperty("Value", QString::number(resultado->bonificacionPC));

        QAxObject * celdaBonusTotal = excel->querySubObject("Cells(Int, Int)", fila,  PLANTILLA_RESULTADOS_TOTAL_BONIFICACION);
        celdaBonusTotal->setProperty("Value", QString::number(resultado->getBonificacionTotal()));
        fila++;
    }
    workbook->querySubObject("SaveAs (const QString&)", path);
    workbook->dynamicCall("Close()");
    excel->dynamicCall( "Quit()");
    delete excel;
}


void ExcelReader::createResultsReport(QString path, QList<ResultadoArbitro*> resultados, QString categoria)
{

    QAxObject* excel = new QAxObject( "Excel.Application", 0 );
    QAxObject* workbooks = excel->querySubObject( "Workbooks" );
    workbooks->querySubObject( "Open(const QString&)", "c:\\pruebas\\template.xlsx");
    QAxObject* workbook = excel->querySubObject("ActiveWorkBook");
    QAxObject* worksheet = workbook->querySubObject("Worksheets(int)", 1);
    QAxObject* usedrange = worksheet->querySubObject("UsedRange");
    QAxObject* rows = usedrange->querySubObject("Rows");
    int intRowStart = PLANTILLA_ARBITROS_FILA_INICIO;
    int intRows = rows->property("Count").toInt();

    QAxObject * celdaTitulo = excel->querySubObject("Cells(Int, Int)", 4,  5);
    celdaTitulo->setProperty("Value", "RESULTADOS DE PRUEBAS FÍSICAS ÁRBITROS " + getTitulo(categoria));

    QAxObject * celdaFecha = excel->querySubObject("Cells(Int, Int)", 4,  18);
    celdaFecha->setProperty("Value", QDateTime::currentDateTime().toString("dd-MMMM-yy"));


    int fila = 8;
    for (ResultadoArbitro* resultado : resultados) {

        QAxObject * celdaDorsal = excel->querySubObject("Cells(Int, Int)", fila,  PLANTILLA_RESULTADOS_DORSAL);
        celdaDorsal->setProperty("Value", QString::number(resultado->arbitro->dorsal));

        QAxObject * celdaNombre = excel->querySubObject("Cells(Int, Int)", fila,  PLANTILLA_RESULTADOS_NOMBRE);
        celdaNombre->setProperty("Value", resultado->arbitro->name);

        int seconds = 0;
        int mseconds = 0;

        for (int i = 0; i < resultado->lista6x40.size(); i++) {
            QAxObject * celdaC6x40 = excel->querySubObject("Cells(Int, Int)", fila,  PLANTILLA_RESULTADOS_C6x40 + i);
            seconds = resultado->lista6x40.at(i).second();
            mseconds = resultado->lista6x40.at(i).msec();
            celdaC6x40->setProperty("Value", QString::number(seconds) + "," + QString::number(mseconds));
        }
        QAxObject * celdaPromedioC6x40 = excel->querySubObject("Cells(Int, Int)", fila,  PLANTILLA_RESULTADOS_PROMEDIO_C6x40);
        seconds = resultado->promedio.second();
        mseconds = resultado->promedio.msec();
        celdaPromedioC6x40->setProperty("Value", QString::number(seconds) + "," + QString::number(mseconds));

        QAxObject * celdaBonusC6x40 = excel->querySubObject("Cells(Int, Int)", fila,  PLANTILLA_RESULTADOS_BONIFICACION_C6x40);
        celdaBonusC6x40->setProperty("Value", QString::number(resultado->bonificacion6x40));

        QAxObject * celdaC2000 = excel->querySubObject("Cells(Int, Int)", fila,  PLANTILLA_RESULTADOS_C2000);
        celdaC2000->setProperty("Value", QString::number(resultado->getResultado2000()));

        QAxObject * celdaBonusC2000 = excel->querySubObject("Cells(Int, Int)", fila,  PLANTILLA_RESULTADOS_BONIFICACION_C2000);
        celdaBonusC2000->setProperty("Value", QString::number(resultado->bonificacion2000));

        QString resultadoPC = "";
        if (resultado->resultadoPC.minute() > 0) {
            resultadoPC = QString::number(resultado->resultadoPC.minute()) + ":";
        }
        resultadoPC = resultadoPC + QString::number(resultado->resultadoPC.second()) + "," + QString::number(resultado->resultadoPC.msec());

        QAxObject * celdaCPC = excel->querySubObject("Cells(Int, Int)", fila,  PLANTILLA_RESULTADOS_CPC);
        celdaCPC->setProperty("Value", resultadoPC);

        QAxObject * celdaBonusCPC = excel->querySubObject("Cells(Int, Int)", fila,  PLANTILLA_RESULTADOS_BONIFICACION_CPC);
        celdaBonusCPC->setProperty("Value", QString::number(resultado->bonificacionPC));

        QAxObject * celdaBonusTotal = excel->querySubObject("Cells(Int, Int)", fila,  PLANTILLA_RESULTADOS_TOTAL_BONIFICACION);
        celdaBonusTotal->setProperty("Value", QString::number(resultado->getBonificacionTotal()));
        fila++;
    }
    workbook->querySubObject("SaveAs (const QString&)", path);
    workbook->dynamicCall("Close()");
    excel->dynamicCall( "Quit()");
    delete excel;

}

void ExcelReader::readBonus(QString categoria, QString sexo, QString pathFile)
{
    QMap<SPORT_TEST, QList<Bonus*>> bonusCategoria;
    QAxObject* excel = new QAxObject( "Excel.Application", 0 );
    QAxObject* workbooks = excel->querySubObject( "Workbooks" );
    workbooks->querySubObject( "Open(const QString&)", pathFile);//"C:\\QtWorkspace\\PLANTILLA ARBITROS OFICIALES.xlsx" );
    QAxObject* workbook = excel->querySubObject("ActiveWorkBook");
    //QAxObject* worksheets = workbook->querySubObject( "Worksheets" );
    QAxObject* worksheet = workbook->querySubObject("Worksheets(int)", 1); //worksheet number

    QAxObject* usedrange = worksheet->querySubObject("UsedRange");
    QAxObject* rows = usedrange->querySubObject("Rows");
    //QAxObject* columns = usedrange->querySubObject("Columns");
    int intRowStart = PLANTILLA_BONUS_FILA_INICIO; //usedrange->property("Row").toInt();
    //int intColStart = usedrange->property("Column").toInt();
    //int intCols = columns->property("Count").toInt();
    int intRows = rows->property("Count").toInt();

    bool primero = true;
    int contador = 1;
    for (int i = intRowStart; i < intRowStart + intRows; i++)
    {

        QAxObject * cell = excel->querySubObject("Cells(Int, Int)", i,  PLANTILLA_BONUS_COL_INICIO);

        QVariant valueVariant =  cell->dynamicCall("value");

        SPORT_TEST typeTest;// = DataManagement::getInstance()->getTipoPrueba(valueVariant.toString());

        if (DataManagement::getInstance()->getTipoPrueba(valueVariant.toString()) == NINGUNA_PRUEBA) {
            //leer los tiempos
            QAxObject * cellTFin = excel->querySubObject("Cells(Int, Int)", i,  PLANTILLA_BONUS_COL_INICIO + 1);
            QAxObject * cellPtos = excel->querySubObject("Cells(Int, Int)", i,  PLANTILLA_BONUS_COL_INICIO + 2);
            QVariant variantTFin = cellTFin->dynamicCall("value");
            QVariant variantPtos = cellPtos->dynamicCall("value");

            if (valueVariant.toDouble() == 0 && variantTFin.toDouble() == 0) {
                continue;
            }

            Bonus* bonus = new Bonus();
            bonus->points = variantPtos.toDouble();
            bonus->timeInit = valueVariant.toDouble();
            bonus->timeEnd = variantTFin.toDouble();


            if (sexo.trimmed().toUpper() == "MASCULINO") {
                DataManagement::getInstance()->addBonus(categoria, typeTest, bonus);
            } else {
                 DataManagement::getInstance()->addBonusFem(categoria, typeTest, bonus);
            }

            //qDebug() << "T.I. " << valueVariant.toString() << " T.F. " <<  variantTFin.toString() << " PTO: " << cariantPtos.toString();
        } else {
            typeTest = DataManagement::getInstance()->getTipoPrueba(valueVariant.toString());
            //qDebug() << "------ " << valueVariant.toString();
            if (primero) {
                primero = false;
                i++;
            }

        }
    }
    workbook->dynamicCall("Close()");
    excel->dynamicCall( "Quit()");
    delete excel;
   // QList<Bonus*>* bonusList = NULL;
   // return bonusList;
}
