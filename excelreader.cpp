#include "excelreader.h"
#include <qdebug.h>

ExcelReader::ExcelReader()
{

}

void ExcelReader::readReferees(QString pathFile)
{
    QAxObject* excel = new QAxObject( "Excel.Application", 0 );
    QAxObject* workbooks = excel->querySubObject( "Workbooks" );
    workbooks->querySubObject( "Open(const QString&)", pathFile);//"C:\\QtWorkspace\\PLANTILLA ARBITROS OFICIALES.xlsx" );
    QAxObject* workbook = excel->querySubObject("ActiveWorkBook");
    //QAxObject* worksheets = workbook->querySubObject( "Worksheets" );
    QAxObject* worksheet = workbook->querySubObject("Worksheets(int)", 1); //worksheet number

    QAxObject* usedrange = worksheet->querySubObject("UsedRange");
    QAxObject* rows = usedrange->querySubObject("Rows");
    //QAxObject* columns = usedrange->querySubObject("Columns");
    int intRowStart = PLANTILLA_ARBITROS_FILA_INICIO; //usedrange->property("Row").toInt();
    //int intColStart = usedrange->property("Column").toInt();
    //int intCols = columns->property("Count").toInt();
    int intRows = rows->property("Count").toInt();

    SessionManagement session;
    QMap<int, QString> mapDorsalesTags = session.loadAsignacionesDorsales();

    int contador = 1;
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
        //referee->born = cellValueFNac.DateTime;
        referee->dorsal = cellValueDorsal.toInt();
        referee->name = cellValueNombre.toString().simplified();
        referee->comarca = cellValueComarca.toString().simplified();
        referee->gender = cellValueSexo.toString().simplified().toUpper();
        referee->categoria = cellValueCategoria.toString().trimmed().toUpper();
        DataManagement::getInstance()->addReferee(referee, findTagInMap(referee->dorsal, mapDorsalesTags));
        contador++;
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

void ExcelReader::createResultsReport(QString path, QList<ResultadoArbitro> resultados)
{

}

void ExcelReader::readBonus(QString categoria, QString pathFile)
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



            DataManagement::getInstance()->addBonus(categoria, typeTest, bonus);

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
