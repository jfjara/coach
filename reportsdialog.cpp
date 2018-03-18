#include "reportsdialog.h"
#include "ui_reportsdialog.h"

ReportsDialog::ReportsDialog(QString path, bool p6x40, bool p5x40, bool p2000, bool pc, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ReportsDialog)
{
    this->path = path;
    this->p6x40 = p6x40;
    this->p5x40 = p5x40;
    this->p2000 = p2000;
    this->pc = pc;

    ui->setupUi(this);

    connect(ui->cancelButton, SIGNAL(clicked(bool)), this, SLOT(cancel()));
    connect(ui->createButton, SIGNAL(clicked(bool)), this, SLOT(createReport()));

    if (!p6x40) {
        ui->p6x40Radio->setEnabled(false);
    }
    if (!p5x40) {
        ui->p5x40Radio->setEnabled(false);
    }
    if (!p2000) {
        ui->p2000Radio->setEnabled(false);
    }
    if (!pc) {
        ui->pcRadio->setEnabled(false);
    }
    if ((p6x40 &&  p2000 && pc) || (p5x40 && p2000 && pc)) {
        ui->totalRadio->setEnabled(true);
    } else {
        ui->totalRadio->setEnabled(false);
    }
}

ReportsDialog::~ReportsDialog()
{
    delete ui;
}

void ReportsDialog::cancel()
{
    this->close();
}

void ReportsDialog::createReport()
{
    if (!ui->separarCatCheck->isChecked()) {
        if (ui->totalRadio->isChecked()) {
            createReportTotal();
        } else if (ui->p6x40Radio->isChecked()) {
            createReport6x40();
        } else if (ui->p5x40Radio->isChecked()) {
            createReport5x40();
        } else if (ui->p2000Radio->isChecked()) {
            createReport2000();
        } else if (ui->pcRadio->isChecked()) {
            createReportPC();
        }
    } else {
        if (ui->totalRadio->isChecked()) {
            createReportCategoriaTotal();
        } else if (ui->p6x40Radio->isChecked()) {
            createReportCategoria6x40();
        } else if (ui->p5x40Radio->isChecked()) {
            createReportCategoria5x40();
        } else if (ui->p2000Radio->isChecked()) {
            createReportCategoria2000();
        } else if (ui->pcRadio->isChecked()) {
            createReportCategoriaPC();
        }
    }
}

bool ReportsDialog::estaApta(SPORT_TEST tipo, ResultadoArbitro* resultado)
{
    if (tipo == C6X40) {
        if (resultado->lista6x40.size() < 6) {
            resultado->apto6x40 = false;
            return false;
        }
    }
    if (tipo == C5X40) {
        if (resultado->lista6x40.size() < 5) {
            resultado->apto5x40 = false;
            return false;
        }
    }
    if (tipo == C2000MTS) {
        if (resultado->resultado2000.size() < 6) {
            resultado->apto2000= false;
            return false;
        }
    }
    if (tipo == PRUEBA_DE_CAMPO) {
        if (resultado->resultadosPC.size() < 2) {
            resultado->aptoPC= false;
            return false;
        }
    }
    return true;
}

void ReportsDialog::calcularPromedioC6x40(ResultadoArbitro* resultado)
{
    int msecs = 0;
    for (QTime r : resultado->lista6x40) {
        msecs += r.second() * 1000 + r.msec();
    }
    int msec6x40 = 0;
    if (resultado->lista6x40.size() > 0) {
        msec6x40 = msecs / resultado->lista6x40.size();
    }

    int secsPromedio = (msec6x40 / 1000);
    int msecPromedio = msec6x40 - (secsPromedio * 1000);

    resultado->promedio = resultado->promedio.addSecs(secsPromedio);
    resultado->promedio = resultado->promedio.addMSecs(msecPromedio);
    resultado->bonificacion6x40 = DataManagement::getInstance()->getBonificacion(resultado->arbitro->categoria, resultado->arbitro->gender, C6X40, resultado->promedio.second() * 1000 + resultado->promedio.msec());
}

void ReportsDialog::createReport6x40()
{
    if (path == Q_NULLPTR || path.isEmpty()) {
        return;
    }
    SessionManagement session;
    QMap<int, QList<QTime>> map6x40 = session.loadPrueba(path  + "\\6x40.ses");

    QList<ResultadoArbitro*> resultados;
    for (Referee* arbitro : DataManagement::getInstance()->refereesMap.values())
    {
        ResultadoArbitro* resultado = new ResultadoArbitro();
        if (map6x40.contains(arbitro->dorsal)) {
            resultado->lista6x40 = map6x40.value(arbitro->dorsal);
        }
        resultado->arbitro = arbitro;
        resultados.append(resultado);
    }
    if (!resultados.isEmpty()) {
        for (ResultadoArbitro* resultado : resultados) {

            if (estaApta(C6X40, resultado)) {
                calcularPromedioC6x40(resultado);
            }
        }

        if (ui->radioButtonOrderDorsal->isChecked()) {
            qSort(resultados.begin(), resultados.end(), ReportsDialog::dorsalMayorQue);
        } else if (ui->radioButtonOrderNombre->isChecked()) {
            qSort(resultados.begin(), resultados.end(), ReportsDialog::nombreMayorQue);
        } else if (ui->radioButtonOrderResultado->isChecked()) {
            qSort(resultados.begin(), resultados.end(), ReportsDialog::promedio6x40MayorQue);
        }

        ExcelReader reader;
        reader.create6x40Report(QDir::toNativeSeparators(path + "\\resultados\\resultados_6x40.xlsx"), resultados, "");
        QMessageBox::information(this, "Crear informe de resultados", "Se ha creado el fichero de resultados de la prueba de 6x40 en la carpeta de trabajo correctamente.", QMessageBox::Ok);
    }
}

QList<ResultadoArbitro*> ReportsDialog::filtrarResultados(QString categoria, QList<ResultadoArbitro*> lista)
{
    QList<ResultadoArbitro*> filtrado;
    for (ResultadoArbitro* resultado : lista) {
        if (resultado->arbitro->categoria.trimmed().toUpper() == categoria) {
            filtrado.append(resultado);
        }
    }
    return filtrado;
}

void ReportsDialog::createReportCategoriaTotal()
{
    SessionManagement session;
    QMap<int, QList<QTime>> map6x40 = session.loadPrueba(QDir::toNativeSeparators(path)  + "\\6x40.ses");
    QMap<int, QList<QTime>> map2000 = session.loadPrueba(QDir::toNativeSeparators(path)  + "\\2000.ses");
    QMap<int, QList<QTime>> mapPC = session.loadPrueba(QDir::toNativeSeparators(path)  + "\\pc.ses");

    QList<ResultadoArbitro*> resultados;
    for (Referee* arbitro : DataManagement::getInstance()->refereesMap.values())
    {        
        ResultadoArbitro* resultado = new ResultadoArbitro();
        if (map6x40.contains(arbitro->dorsal)) {
            resultado->lista6x40 = map6x40.value(arbitro->dorsal);
        }

        if (map2000.contains(arbitro->dorsal)) {
            resultado->resultado2000 = map2000.value(arbitro->dorsal);
        }

        if (mapPC.contains(arbitro->dorsal)) {
            resultado->resultadosPC = mapPC.value(arbitro->dorsal);
            if (resultado->resultadosPC.size() > 1) {
                resultado->resultadoPC = mapPC.value(arbitro->dorsal).at(1);
            }
        }
        resultado->arbitro = arbitro;
        resultados.append(resultado);
    }

    if (!resultados.isEmpty()) {
        for (ResultadoArbitro* resultado : resultados) {
            if (estaApta(C6X40, resultado)) {
                calcularPromedioC6x40(resultado);
            }
            if (estaApta(C2000MTS, resultado)) {
                resultado->bonificacion2000 = DataManagement::getInstance()->getBonificacion(resultado->arbitro->categoria, resultado->arbitro->gender,
                                                                                        C2000MTS, resultado->getResultado2000());
            }
            if (estaApta(PRUEBA_DE_CAMPO, resultado)) {
                resultado->bonificacionPC = DataManagement::getInstance()->getBonificacion(resultado->arbitro->categoria, resultado->arbitro->gender,
                                                                                          PRUEBA_DE_CAMPO, (resultado->resultadoPC.minute() * 6000) + (resultado->resultadoPC.second() * 1000) + resultado->resultadoPC.msec());
            }
        }
    }

    crearInformeCategoria("OFICIAL", resultados);
    crearInformeCategoria("ASISTENTE2B", resultados);
    crearInformeCategoria("3DIVISION", resultados);
    crearInformeCategoria("ASISTENTE3DIVISION", resultados);
    crearInformeCategoria("DIVISIONHONORSENIOR", resultados);
    crearInformeCategoria("PROVINCIAL", resultados);
    crearInformeCategoria("NUEVOINGRESO", resultados);
    crearInformeCategoria("FORMADORES", resultados);
    crearInformeCategoria("FUTBOLSALA", resultados);


    QMessageBox::information(this, "Crear informe de resultados", "Se han creado los ficheros de resultados de las pruebas en la carpeta de trabajo correctamente.", QMessageBox::Ok);
}

void ReportsDialog::createReportCategoria6x40()
{
    SessionManagement session;
    QMap<int, QList<QTime>> map6x40 = session.loadPrueba(QDir::toNativeSeparators(path)  + "\\6x40.ses");

    QList<ResultadoArbitro*> resultados;
    for (Referee* arbitro : DataManagement::getInstance()->refereesMap.values())
    {
        ResultadoArbitro* resultado = new ResultadoArbitro();
        if (map6x40.contains(arbitro->dorsal)) {
            resultado->lista6x40 = map6x40.value(arbitro->dorsal);
        }
        resultado->arbitro = arbitro;
        resultados.append(resultado);
    }

    if (!resultados.isEmpty()) {
        for (ResultadoArbitro* resultado : resultados) {
            if (estaApta(C6X40, resultado)) {
                calcularPromedioC6x40(resultado);
            }
         }
    }

    crearInformeCategoria("OFICIAL", resultados);
    crearInformeCategoria("ASISTENTE2B", resultados);
    crearInformeCategoria("3DIVISION", resultados);
    crearInformeCategoria("ASISTENTE3DIVISION", resultados);
    crearInformeCategoria("DIVISIONHONORSENIOR", resultados);
    crearInformeCategoria("PROVINCIAL", resultados);
    crearInformeCategoria("NUEVOINGRESO", resultados);
    crearInformeCategoria("FORMADORES", resultados);
    crearInformeCategoria("FUTBOLSALA", resultados);

    QMessageBox::information(this, "Crear informe de resultados", "Se han creado los ficheros de resultados de las pruebas en la carpeta de trabajo correctamente.", QMessageBox::Ok);
}

void ReportsDialog::createReportCategoria5x40()
{
    SessionManagement session;
    QMap<int, QList<QTime>> map6x40 = session.loadPrueba(QDir::toNativeSeparators(path)  + "\\5x40.ses");

    QList<ResultadoArbitro*> resultados;
    for (Referee* arbitro : DataManagement::getInstance()->refereesMap.values())
    {
        ResultadoArbitro* resultado = new ResultadoArbitro();
        if (map6x40.contains(arbitro->dorsal)) {
            resultado->lista6x40 = map6x40.value(arbitro->dorsal);
        }
        resultado->arbitro = arbitro;
        resultados.append(resultado);
    }

    if (!resultados.isEmpty()) {
        for (ResultadoArbitro* resultado : resultados) {
            if (estaApta(C5X40, resultado)) {
                calcularPromedioC6x40(resultado);
            }
         }
    }

    crearInformeCategoria("OFICIAL", resultados);
    crearInformeCategoria("ASISTENTE2B", resultados);
    crearInformeCategoria("3DIVISION", resultados);
    crearInformeCategoria("ASISTENTE3DIVISION", resultados);
    crearInformeCategoria("DIVISIONHONORSENIOR", resultados);
    crearInformeCategoria("PROVINCIAL", resultados);
    crearInformeCategoria("NUEVOINGRESO", resultados);
    crearInformeCategoria("FORMADORES", resultados);
    crearInformeCategoria("FUTBOLSALA", resultados);

    QMessageBox::information(this, "Crear informe de resultados", "Se han creado los ficheros de resultados de las pruebas en la carpeta de trabajo correctamente.", QMessageBox::Ok);
}

void ReportsDialog::createReportCategoria2000()
{
    SessionManagement session;

    QMap<int, QList<QTime>> map2000 = session.loadPrueba(QDir::toNativeSeparators(path)  + "\\2000.ses");

    QList<ResultadoArbitro*> resultados;
    for (Referee* arbitro : DataManagement::getInstance()->refereesMap.values())
    {
        ResultadoArbitro* resultado = new ResultadoArbitro();

        if (map2000.contains(arbitro->dorsal)) {
            resultado->resultado2000 = map2000.value(arbitro->dorsal);
        }

        resultado->arbitro = arbitro;
        resultados.append(resultado);
    }

    if (!resultados.isEmpty()) {
        for (ResultadoArbitro* resultado : resultados) {
            if (estaApta(C2000MTS, resultado)) {
                resultado->bonificacion2000 = DataManagement::getInstance()->getBonificacion(resultado->arbitro->categoria, resultado->arbitro->gender,
                                                                                        C2000MTS, resultado->getResultado2000());
            }
        }
    }

    crearInformeCategoria("OFICIAL", resultados);
    crearInformeCategoria("ASISTENTE2B", resultados);
    crearInformeCategoria("3DIVISION", resultados);
    crearInformeCategoria("ASISTENTE3DIVISION", resultados);
    crearInformeCategoria("DIVISIONHONORSENIOR", resultados);
    crearInformeCategoria("PROVINCIAL", resultados);
    crearInformeCategoria("NUEVOINGRESO", resultados);
    crearInformeCategoria("FORMADORES", resultados);
    crearInformeCategoria("FUTBOLSALA", resultados);

    QMessageBox::information(this, "Crear informe de resultados", "Se han creado los ficheros de resultados de las pruebas en la carpeta de trabajo correctamente.", QMessageBox::Ok);
}

void ReportsDialog::createReportCategoriaPC()
{
    SessionManagement session;
    QMap<int, QList<QTime>> mapPC = session.loadPrueba(QDir::toNativeSeparators(path)  + "\\pc.ses");

    QList<ResultadoArbitro*> resultados;
    for (Referee* arbitro : DataManagement::getInstance()->refereesMap.values())
    {
        ResultadoArbitro* resultado = new ResultadoArbitro();

        if (mapPC.contains(arbitro->dorsal)) {
            resultado->resultadosPC = mapPC.value(arbitro->dorsal);
            if (resultado->resultadosPC.size() > 1) {
                resultado->resultadoPC = mapPC.value(arbitro->dorsal).at(1);
            }
        }
        resultado->arbitro = arbitro;
        resultados.append(resultado);
    }

    if (!resultados.isEmpty()) {
        for (ResultadoArbitro* resultado : resultados) {            
            if (estaApta(PRUEBA_DE_CAMPO, resultado)) {
                resultado->bonificacionPC = DataManagement::getInstance()->getBonificacion(resultado->arbitro->categoria, resultado->arbitro->gender,
                                                                                          PRUEBA_DE_CAMPO, (resultado->resultadoPC.minute() * 6000) + (resultado->resultadoPC.second() * 1000) + resultado->resultadoPC.msec());
            }
        }
    }

    crearInformeCategoria("OFICIAL", resultados);
    crearInformeCategoria("ASISTENTE2B", resultados);
    crearInformeCategoria("3DIVISION", resultados);
    crearInformeCategoria("ASISTENTE3DIVISION", resultados);
    crearInformeCategoria("DIVISIONHONORSENIOR", resultados);
    crearInformeCategoria("PROVINCIAL", resultados);
    crearInformeCategoria("NUEVOINGRESO", resultados);
    crearInformeCategoria("FORMADORES", resultados);
    crearInformeCategoria("FUTBOLSALA", resultados);

    QMessageBox::information(this, "Crear informe de resultados", "Se han creado los ficheros de resultados de las pruebas en la carpeta de trabajo correctamente.", QMessageBox::Ok);
}

void ReportsDialog::crearInformeCategoria(QString categoria,  QList<ResultadoArbitro*> resultados)
{

    QList<ResultadoArbitro*> filtrados = filtrarResultados(categoria, resultados);
    if (filtrados.isEmpty()) {
        return;
    }
    if (ui->radioButtonOrderDorsal->isChecked()) {
        qSort(filtrados.begin(), filtrados.end(), ReportsDialog::dorsalMayorQue);
    } else if (ui->radioButtonOrderNombre->isChecked()) {
        qSort(filtrados.begin(), filtrados.end(), ReportsDialog::nombreMayorQue);
    } else if (ui->radioButtonOrderResultado->isChecked()) {
        qSort(filtrados.begin(), filtrados.end(), ReportsDialog::promedioMayorQue);
    }

    ExcelReader reader;

    if (ui->totalRadio->isChecked()) {
        reader.createResultsReport(QDir::toNativeSeparators(path + "\\resultados\\" + categoria + "_resultados.xlsx"), filtrados, categoria);
    } else if (ui->p6x40Radio->isChecked()) {
        reader.create6x40Report(QDir::toNativeSeparators(path + "\\resultados\\" + categoria + "_6x40.xlsx"), filtrados, categoria);
    } else if (ui->p5x40Radio->isChecked()) {
        reader.create5x40Report(QDir::toNativeSeparators(path + "\\resultados\\" + categoria + "_5x40.xlsx"), filtrados, categoria);
    } else if (ui->p2000Radio->isChecked()) {
        reader.create2000Report(QDir::toNativeSeparators(path + "\\resultados\\" + categoria + "_2000.xlsx"), filtrados, categoria);
    } else if (ui->pcRadio->isChecked()) {
        reader.createPCReport(QDir::toNativeSeparators(path + "\\resultados\\" + categoria + "_pruebaCampo.xlsx"), filtrados, categoria);
    }

    //reader.createResultsReport(QDir::toNativeSeparators(path), filtrados);

}

void ReportsDialog::createReportTotal()
{
    if (path == Q_NULLPTR || path.isEmpty()) {
        return;
    }

    SessionManagement session;
    QMap<int, QList<QTime>> map6x40 = session.loadPrueba(QDir::toNativeSeparators(path)  + "\\6x40.ses");
    QMap<int, QList<QTime>> map2000 = session.loadPrueba(QDir::toNativeSeparators(path)  + "\\2000.ses");
    QMap<int, QList<QTime>> mapPC = session.loadPrueba(QDir::toNativeSeparators(path)  + "\\pc.ses");

    QList<ResultadoArbitro*> resultados;
    for (Referee* arbitro : DataManagement::getInstance()->refereesMap.values())
    {
        ResultadoArbitro* resultado = new ResultadoArbitro();
        if (map6x40.contains(arbitro->dorsal)) {
            resultado->lista6x40 = map6x40.value(arbitro->dorsal);
        }

        if (map2000.contains(arbitro->dorsal)) {
            resultado->resultado2000 = map2000.value(arbitro->dorsal);
        }

        if (mapPC.contains(arbitro->dorsal)) {
            resultado->resultadosPC = mapPC.value(arbitro->dorsal);
            if (resultado->resultadosPC.size() > 1) {
                resultado->resultadoPC = mapPC.value(arbitro->dorsal).at(1);
            }
        }
        resultado->arbitro = arbitro;
        resultados.append(resultado);
    }

    if (!resultados.isEmpty()) {
        for (ResultadoArbitro* resultado : resultados) {
            if (estaApta(C6X40, resultado)) {
                calcularPromedioC6x40(resultado);
            }

            if (estaApta(C2000MTS, resultado)) {
                resultado->bonificacion2000 = DataManagement::getInstance()->getBonificacion(resultado->arbitro->categoria, resultado->arbitro->gender,
                                                                                        C2000MTS, resultado->getResultado2000());
            }

            if (estaApta(PRUEBA_DE_CAMPO, resultado)) {
                resultado->bonificacionPC = DataManagement::getInstance()->getBonificacion(resultado->arbitro->categoria, resultado->arbitro->gender,
                                                                                          PRUEBA_DE_CAMPO, (resultado->resultadoPC.minute() * 6000) + (resultado->resultadoPC.second() * 1000) + resultado->resultadoPC.msec());
            }
        }
        if (ui->radioButtonOrderDorsal->isChecked()) {
            qSort(resultados.begin(), resultados.end(), ReportsDialog::dorsalMayorQue);
        } else if (ui->radioButtonOrderNombre->isChecked()) {
            qSort(resultados.begin(), resultados.end(), ReportsDialog::nombreMayorQue);
        } else if (ui->radioButtonOrderResultado->isChecked()) {
            qSort(resultados.begin(), resultados.end(), ReportsDialog::promedioMayorQue);
        }

        ExcelReader reader;
        reader.createResultsReport(QDir::toNativeSeparators(path + "\\resultados\\resultados.xlsx"), resultados, "");

        QMessageBox::information(this, "Crear informe de resultados", "Se ha creado el fichero de resultados de las pruebas en la carpeta de trabajo correctamente.", QMessageBox::Ok);
    }

}

//TODO: add 5x40
// rutas

void ReportsDialog::createReport5x40()
{
    if (path == Q_NULLPTR || path.isEmpty()) {
        return;
    }
    SessionManagement session;
    QMap<int, QList<QTime>> map5x40 = session.loadPrueba(path  + "\\5x40.ses");

    QList<ResultadoArbitro*> resultados;
    for (Referee* arbitro : DataManagement::getInstance()->refereesMap.values())
    {
        ResultadoArbitro* resultado = new ResultadoArbitro();
        if (map5x40.contains(arbitro->dorsal)) {
            resultado->lista6x40 = map5x40.value(arbitro->dorsal);
        }
        resultado->arbitro = arbitro;
        resultados.append(resultado);
    }
    if (!resultados.isEmpty()) {
        for (ResultadoArbitro* resultado : resultados) {
            if (estaApta(C5X40, resultado)) {
                calcularPromedioC6x40(resultado);
            }
        }

        if (ui->radioButtonOrderDorsal->isChecked()) {
            qSort(resultados.begin(), resultados.end(), ReportsDialog::dorsalMayorQue);
        } else if (ui->radioButtonOrderNombre->isChecked()) {
            qSort(resultados.begin(), resultados.end(), ReportsDialog::nombreMayorQue);
        } else if (ui->radioButtonOrderResultado->isChecked()) {
            qSort(resultados.begin(), resultados.end(), ReportsDialog::promedio6x40MayorQue);
        }

        ExcelReader reader;
        reader.create5x40Report(QDir::toNativeSeparators(path + "\\resultados\\resultados_5x40.xlsx"), resultados, "");
        QMessageBox::information(this, "Crear informe de resultados", "Se ha creado el fichero de resultados de la prueba de 6x40 en la carpeta de trabajo correctamente.", QMessageBox::Ok);
    }
}

void ReportsDialog::createReport2000()
{

    if (path == Q_NULLPTR || path.isEmpty()) {
        return;
    }
    SessionManagement session;
    QMap<int, QList<QTime>> map2000 = session.loadPrueba(QDir::toNativeSeparators(path)  + "\\2000.ses");

    QList<ResultadoArbitro*> resultados;
    for (Referee* arbitro : DataManagement::getInstance()->refereesMap.values())
    {
        ResultadoArbitro* resultado = new ResultadoArbitro();
        if (map2000.contains(arbitro->dorsal)) {
            resultado->resultado2000 = map2000.value(arbitro->dorsal);
        }
        resultado->arbitro = arbitro;
        resultados.append(resultado);
    }
    if (!resultados.isEmpty()) {
        for (ResultadoArbitro* resultado : resultados) {
            if (estaApta(C2000MTS, resultado)) {
                resultado->bonificacion2000 = DataManagement::getInstance()->getBonificacion(resultado->arbitro->categoria, resultado->arbitro->gender,
                                                                                        C2000MTS, resultado->getResultado2000());
            }
        }
        if (ui->radioButtonOrderDorsal->isChecked()) {
            qSort(resultados.begin(), resultados.end(), ReportsDialog::dorsalMayorQue);
        } else if (ui->radioButtonOrderNombre->isChecked()) {
            qSort(resultados.begin(), resultados.end(), ReportsDialog::nombreMayorQue);
        } else if (ui->radioButtonOrderResultado->isChecked()) {
            qSort(resultados.begin(), resultados.end(), ReportsDialog::promedio2000MayorQue);
        }

        ExcelReader reader;
        reader.create2000Report(QDir::toNativeSeparators(path + "\\resultados\\resultados_2000.xlsx"), resultados, "");

        QMessageBox::information(this, "Crear informe de resultados", "Se ha creado el fichero de resultados de las pruebas de 2000 metros en la carpeta de trabajo correctamente.", QMessageBox::Ok);
    }
}

void ReportsDialog::createReportPC()
{
    if (path == Q_NULLPTR || path.isEmpty()) {
        return;
    }
    SessionManagement session;
    QMap<int, QList<QTime>> mapPC = session.loadPrueba(QDir::toNativeSeparators(path)  + "\\pc.ses");

    QList<ResultadoArbitro*> resultados;
    for (Referee* arbitro : DataManagement::getInstance()->refereesMap.values())
    {
        ResultadoArbitro* resultado = new ResultadoArbitro();
        if (mapPC.contains(arbitro->dorsal)) {
            resultado->resultadosPC = mapPC.value(arbitro->dorsal);
            if (resultado->resultadosPC.size() > 1) {
                resultado->resultadoPC = mapPC.value(arbitro->dorsal).at(1);
            }
        }
        resultado->arbitro = arbitro;
        resultados.append(resultado);
    }
    if (!resultados.isEmpty()) {
        for (ResultadoArbitro* resultado : resultados) {
            if (estaApta(PRUEBA_DE_CAMPO, resultado)) {
                resultado->bonificacionPC = DataManagement::getInstance()->getBonificacion(resultado->arbitro->categoria, resultado->arbitro->gender,
                                                                                          PRUEBA_DE_CAMPO, (resultado->resultadoPC.minute() * 6000) + (resultado->resultadoPC.second() * 1000) + resultado->resultadoPC.msec());
            }
        }
        if (ui->radioButtonOrderDorsal->isChecked()) {
            qSort(resultados.begin(), resultados.end(), ReportsDialog::dorsalMayorQue);
        } else if (ui->radioButtonOrderNombre->isChecked()) {
            qSort(resultados.begin(), resultados.end(), ReportsDialog::nombreMayorQue);
        } else if (ui->radioButtonOrderResultado->isChecked()) {
            qSort(resultados.begin(), resultados.end(), ReportsDialog::promedioPCMayorQue);
        }

        ExcelReader reader;
        reader.createPCReport(QDir::toNativeSeparators(path + "\\resultados\\resultados_pruebaCampo.xlsx"), resultados, "");

        QMessageBox::information(this, "Crear informe de resultados", "Se ha creado el fichero de resultados de la prueba de campo en la carpeta de trabajo correctamente.", QMessageBox::Ok);
    }
}
