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
            int msecs = 0;
            for (QTime r : resultado->lista6x40) {
                msecs += r.second() * 1000 + r.msec();
            }
            int msec6x40 = msecs / resultado->lista6x40.size();

            int secsPromedio = (msec6x40 / 1000);
            int msecPromedio = msec6x40 - (secsPromedio * 1000);

            resultado->promedio = resultado->promedio.addSecs(secsPromedio);
            resultado->promedio = resultado->promedio.addMSecs(msecPromedio);

            resultado->bonificacion6x40 = DataManagement::getInstance()->getBonificacion(resultado->arbitro->categoria, resultado->arbitro->gender, C6X40, resultado->promedio.second() * 1000 + resultado->promedio.msec());
        }

        if (ui->radioButtonOrderDorsal->isChecked()) {
            qSort(resultados.begin(), resultados.end(), ReportsDialog::dorsalMayorQue);
        } else if (ui->radioButtonOrderNombre->isChecked()) {
            qSort(resultados.begin(), resultados.end(), ReportsDialog::nombreMayorQue);
        } else if (ui->radioButtonOrderResultado->isChecked()) {
            qSort(resultados.begin(), resultados.end(), ReportsDialog::promedio6x40MayorQue);
        }



        ExcelReader reader;
        reader.create6x40Report(QDir::toNativeSeparators(path), resultados);

        QMessageBox::information(this, "Crear informe de resultados", "Se ha creado el fichero de resultados de la prueba de 6x40 en la carpeta de trabajo correctamente.", QMessageBox::Ok);
    }\
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
            resultado->resultadoPC = mapPC.value(arbitro->dorsal).at(1);
        }
        resultado->arbitro = arbitro;
        resultados.append(resultado);
    }

    if (!resultados.isEmpty()) {
        for (ResultadoArbitro* resultado : resultados) {
            int msecs = 0;
            for (QTime r : resultado->lista6x40) {
                msecs += r.second() * 1000 + r.msec();
            }
            int msec6x40 = msecs / resultado->lista6x40.size();

            int secsPromedio = (msec6x40 / 1000);
            int msecPromedio = msec6x40 - (secsPromedio * 1000);

            resultado->promedio = resultado->promedio.addSecs(secsPromedio);
            resultado->promedio = resultado->promedio.addMSecs(msecPromedio);

            resultado->bonificacion6x40 = DataManagement::getInstance()->getBonificacion(resultado->arbitro->categoria, resultado->arbitro->gender,
                                                                                        C6X40, resultado->promedio.second() * 1000 + resultado->promedio.msec());
            resultado->bonificacion2000 = DataManagement::getInstance()->getBonificacion(resultado->arbitro->categoria, resultado->arbitro->gender,
                                                                                        C2000MTS, resultado->getResultado2000());


            resultado->bonificacionPC = DataManagement::getInstance()->getBonificacion(resultado->arbitro->categoria, resultado->arbitro->gender,
                                                                                      PRUEBA_DE_CAMPO, (resultado->resultadoPC.minute() * 6000) + (resultado->resultadoPC.second() * 1000) + resultado->resultadoPC.msec());
        }
        if (ui->radioButtonOrderDorsal->isChecked()) {
            qSort(resultados.begin(), resultados.end(), ReportsDialog::dorsalMayorQue);
        } else if (ui->radioButtonOrderNombre->isChecked()) {
            qSort(resultados.begin(), resultados.end(), ReportsDialog::nombreMayorQue);
        } else if (ui->radioButtonOrderResultado->isChecked()) {
            qSort(resultados.begin(), resultados.end(), ReportsDialog::promedioMayorQue);
        }

        ExcelReader reader;
        reader.createResultsReport(QDir::toNativeSeparators(path), resultados);

        QMessageBox::information(this, "Crear informe de resultados", "Se ha creado el fichero de resultados de las pruebas en la carpeta de trabajo correctamente.", QMessageBox::Ok);
    }

}

void ReportsDialog::createReport5x40()
{

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
            resultado->bonificacion2000 = DataManagement::getInstance()->getBonificacion(resultado->arbitro->categoria, resultado->arbitro->gender,
                                                                                        C2000MTS, resultado->getResultado2000());
        }
        if (ui->radioButtonOrderDorsal->isChecked()) {
            qSort(resultados.begin(), resultados.end(), ReportsDialog::dorsalMayorQue);
        } else if (ui->radioButtonOrderNombre->isChecked()) {
            qSort(resultados.begin(), resultados.end(), ReportsDialog::nombreMayorQue);
        } else if (ui->radioButtonOrderResultado->isChecked()) {
            qSort(resultados.begin(), resultados.end(), ReportsDialog::promedio2000MayorQue);
        }

        ExcelReader reader;
        reader.create2000Report(QDir::toNativeSeparators(path), resultados);

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
            resultado->resultadoPC = mapPC.value(arbitro->dorsal).at(1);
        }
        resultado->arbitro = arbitro;
        resultados.append(resultado);
    }
    if (!resultados.isEmpty()) {
        for (ResultadoArbitro* resultado : resultados) {
            resultado->bonificacionPC = DataManagement::getInstance()->getBonificacion(resultado->arbitro->categoria, resultado->arbitro->gender,
                                                                                      PRUEBA_DE_CAMPO, (resultado->resultadoPC.minute() * 6000) + (resultado->resultadoPC.second() * 1000) + resultado->resultadoPC.msec());
        }
        if (ui->radioButtonOrderDorsal->isChecked()) {
            qSort(resultados.begin(), resultados.end(), ReportsDialog::dorsalMayorQue);
        } else if (ui->radioButtonOrderNombre->isChecked()) {
            qSort(resultados.begin(), resultados.end(), ReportsDialog::nombreMayorQue);
        } else if (ui->radioButtonOrderResultado->isChecked()) {
            qSort(resultados.begin(), resultados.end(), ReportsDialog::promedioPCMayorQue);
        }

        ExcelReader reader;
        reader.createPCReport(QDir::toNativeSeparators(path), resultados);

        QMessageBox::information(this, "Crear informe de resultados", "Se ha creado el fichero de resultados de la prueba de campo en la carpeta de trabajo correctamente.", QMessageBox::Ok);
    }
}
