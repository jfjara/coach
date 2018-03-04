#include "mycoachmainwindow.h"
#include "ui_mycoachmainwindow.h"

MyCoachMainWindow::MyCoachMainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MyCoachMainWindow)
{
    this->setWindowState(Qt::WindowMaximized);
    ui->setupUi(this);
    ui->layoutChronometre->addWidget(&chronometre);

    QStringList tableHeader;
    tableHeader << "Vuelta" << "Tiempo vuelta" << "Tiempo total" << "Nombre" << "Categoría";
    ui->raceTable->setColumnCount(5);
    ui->raceTable->setHorizontalHeaderLabels(tableHeader);

    ui->raceTable->horizontalHeader()->resizeSection( 0, 60 );
    ui->raceTable->horizontalHeader()->resizeSection( 1, 150 );
    ui->raceTable->horizontalHeader()->resizeSection( 2, 150 );
    ui->raceTable->horizontalHeader()->resizeSection( 3, 400 );
    ui->raceTable->horizontalHeader()->resizeSection( 1, 170 );


    dateAndHourTimer.setInterval(1000);
    dateAndHourTimer.start();;
    updateDateTime();

    mostrarMensajeCargaBonus();

    connect(ui->startButton, SIGNAL(clicked(bool)), this, SLOT(initRace()));
    connect(ui->stopButton, SIGNAL(clicked(bool)), this, SLOT(stopChronometre()));
    connect(ui->refereeButton, SIGNAL(clicked(bool)), this, SLOT(openRefereeDialog()));
    connect(ui->configButton, SIGNAL(clicked(bool)), this, SLOT(openConfigureDialog()));
    connect(ui->reportButton, SIGNAL(clicked(bool)), this, SLOT(createReport()));
    connect(ui->bonusButton, SIGNAL(clicked(bool)), this, SLOT(loadBonusFile()));
    connect(&timerStartRace, SIGNAL(timeout()), this, SLOT(startChonometre()));
    connect(&dateAndHourTimer, SIGNAL(timeout()), this, SLOT(updateDateTime()));
    connect(ui->verBonusButton, SIGNAL(clicked(bool)), this, SLOT(mostrarBonus()));
    connect(ui->radio60x40Button, SIGNAL(clicked(bool)), this, SLOT(configureTest6x40()));
    connect(ui->radio5x40Button, SIGNAL(clicked(bool)), this, SLOT(configureTest5x40()));
    connect(ui->radio2000Button, SIGNAL(clicked(bool)), this, SLOT(configureTest2000()));
    connect(ui->radioPCButton, SIGNAL(clicked(bool)), this, SLOT(configureTestPC()));    
    connect(ui->cargarSesionButton, SIGNAL(clicked(bool)), this, SLOT(loadSession()));
    connect(ui->dorsalTagsButton, SIGNAL(clicked(bool)), this, SLOT(openDorsalesTagsDialog()));

    ui->reportButton->setEnabled(false);

    ui->bonusButton->setVisible(false);

    configureTest6x40();

   // connect(serverSocket.thread, SIGNAL(sendTagValue(QString)), this, SLOT(receiveTag(QString)));

}

MyCoachMainWindow::~MyCoachMainWindow()
{
    delete ui;
}

void MyCoachMainWindow::enableOptions(bool enable)
{
    ui->radio5x40Button->setEnabled(enable);
    ui->radio60x40Button->setEnabled(enable);
    ui->radio2000Button->setEnabled(enable);
    ui->radioPCButton->setEnabled(enable);
    ui->startButton->setEnabled(enable);
    ui->bonusButton->setEnabled(enable);
    ui->cargarSesionButton->setEnabled(enable);
    ui->configButton->setEnabled(enable);
    ui->dorsalTagsButton->setEnabled(enable);
    ui->verBonusButton->setEnabled(enable);
    ui->refereeButton->setEnabled(enable);
    ui->reportButton->setEnabled(enable);
}

void MyCoachMainWindow::newSession()
{
    //TODO: ver como introducir el nombre
}

void MyCoachMainWindow::loadSession()
{
    QString path = QFileDialog::getExistingDirectory(this, tr("Seleccionar carpeta de trabajo)"), QDir::currentPath());

    if (path !=Q_NULLPTR && !path.isEmpty()) {
        cargarBonus("OFICIAL", path + "\\bonificaciones.xlsx");
        cargarBonus("ASISTENTE2B", path + "\\bonificaciones.xlsx");
        cargarBonus("3DIVISION", path + "\\bonificaciones.xlsx");
        cargarBonus("ASISTENTE3DIVISION", path + "\\bonificaciones.xlsx");
        cargarBonus("DIVISIONHONORSENIOR", path + "\\bonificaciones.xlsx");
        cargarBonus("PROVINCIAL", path + "\\bonificaciones.xlsx");
        cargarBonus("NUEVOINGRESO", path + "\\bonificaciones.xlsx");


        // asistente 2b, 3 division, asistente 3 division, division honor senior, provincial, oficial, nuevo ingreso
        cargarParticipantes(path + "\\participantes.xlsx");
        ui->checkBox6x40->setChecked(checkearFicherosPruebas(path + "//6x40.ses"));
        ui->checkBox5x40->setChecked(checkearFicherosPruebas(path + "//5x40.ses"));
        ui->checkBox2000->setChecked(checkearFicherosPruebas(path + "//2000.ses"));
        ui->checkBoxPC->setChecked(checkearFicherosPruebas(path + "//pc.ses"));
        sessionFilePath = path;

        if (ui->checkBox6x40->isChecked() && ui->checkBox2000->isChecked() && ui->checkBoxPC->isChecked()) {
            ui->reportButton->setEnabled(true);
        } else {
            ui->reportButton->setDisabled(false);
        }

        QMessageBox::information(this, "Datos correctamente cargados", "Se han cargados los ficheros de bonificaciones y participantes correctamente", QMessageBox::Ok);
    }
}

void MyCoachMainWindow::saveSession()
{

}

void MyCoachMainWindow::configureTest6x40()
{
    DataManagement::getInstance()->distanceLap = 40;
    DataManagement::getInstance()->totalLaps = 6;
    updateRaceInfo();
}


void MyCoachMainWindow::configureTest2000()
{
    DataManagement::getInstance()->distanceLap = 400;
    DataManagement::getInstance()->totalLaps = 5;
    updateRaceInfo();
}

void MyCoachMainWindow::configureTest5x40()
{
    DataManagement::getInstance()->distanceLap = 40;
    DataManagement::getInstance()->totalLaps = 5;
    updateRaceInfo();
}

void MyCoachMainWindow::configureTestPC()
{
    DataManagement::getInstance()->distanceLap = 1000;
    DataManagement::getInstance()->totalLaps = 1;
    updateRaceInfo();
}

void MyCoachMainWindow::initRace()
{
    if (DataManagement::getInstance()->refereesMap.size() <= 0) {
        QMessageBox::warning(this, "Error", "No hay personas cargadas para realizar la prueba. Por favor, Cargue el Excel correspondiente.", QMessageBox::Ok);
        return;
    }

    if (DataManagement::getInstance()->bonusMap.size() <= 0) {
        QMessageBox::warning(this, "Error", "No se han cargado las bonificaciones. Por favor, cargue el Excel correspondiente.", QMessageBox::Ok);
        return;
    }


    for (Referee* referee : DataManagement::getInstance()->refereesMap.values()) {
        referee->totalLapsRace = DataManagement::getInstance()->totalLaps;
        referee->finished = false;
        referee->laps.clear();
    }

    enableOptions(false);
    ui->startButton->setEnabled(false);
    countDownDialog.init();
    countDownDialog.start();
    countDownDialog.exec();
    startChonometre();
    serverSocket = new ServerSocket(1, this, Q_NULLPTR, Q_NULLPTR);
    serverSocket->StartServer();

    timerSimulation.setInterval(100);
    connect(&timerSimulation, SIGNAL(timeout()), this, SLOT(checkRace()));
    timerSimulation.start();
}

void MyCoachMainWindow::mostrarMensajeCargaBonus()
{
    if (DataManagement::getInstance()->bonusMap.size() > 0) {
        ui->bonusLabel->setStyleSheet("QLabel { color : green; }");
        ui->bonusLabel->setText("Cargado");
    } else {
        ui->bonusLabel->setStyleSheet("QLabel { color : red; }");
        ui->bonusLabel->setText("No disponible");
    }
}

void MyCoachMainWindow::mostrarBonus()
{
    BonusDialog dialog;
    dialog.exec();
}

void MyCoachMainWindow::updateDateTime()
{
    QTime currentTime = QTime::currentTime();
    QDateTime currentDate = QDateTime::currentDateTime();
    ui->hourLabel->setText(currentTime.toString());
    ui->dateLabel->setText(currentDate.toString("d MMM yyyy"));
}


void MyCoachMainWindow::startChonometre()
{
    clean();
    chronometre.start();
    //startSimulation();
}

void MyCoachMainWindow::stopChronometre()
{
    enableOptions(true);
    timerSimulation.stop();
    generatorTag.stop();
    chronometre.stop();

    SessionManagement session;

    int tipoPrueba = 0;
    if (ui->radio2000Button->isChecked()) {
        tipoPrueba = 1;
        ui->checkBox2000->setChecked(true);
    } else if (ui->radio60x40Button->isChecked()) {
        tipoPrueba = 0;
        ui->checkBox6x40->setChecked(true);
    } else if (ui->radioPCButton->isChecked()) {
        tipoPrueba = 2;
        ui->checkBoxPC->setChecked(true);
    } else if (ui->radio5x40Button->isChecked()) {
        tipoPrueba = 3;
        ui->checkBox5x40->setChecked(true);
    }
    delete serverSocket;
    session.save(sessionFilePath, tipoPrueba);

    if (ui->checkBox6x40->isChecked() && ui->checkBox2000->isChecked() && ui->checkBoxPC->isChecked()) {
        ui->reportButton->setEnabled(true);
    } else {
        ui->reportButton->setDisabled(false);
    }

}

void MyCoachMainWindow::openRefereeDialog()
{
    RefeererDialog dialog;
    dialog.exec();
    updateRaceInfo();

}

void MyCoachMainWindow::openConfigureDialog()
{
    CalibradorDialog dialog;
    dialog.exec();
}

void MyCoachMainWindow::openDorsalesTagsDialog()
{
    DorsalesTagsDialog dialog;
    dialog.exec();
}

void MyCoachMainWindow::updateRaceInfo()
{
    ui->vueltasLabel->setText(QString::number(DataManagement::getInstance()->totalLaps));
    ui->distVueltaLabel->setText(QString::number(DataManagement::getInstance()->distanceLap).append(" m"));
    ui->distTotalLabel->setText(QString::number(DataManagement::getInstance()->totalLaps *
                                                DataManagement::getInstance()->distanceLap).append(" m"));
    ui->corredoresLabel->setText(QString::number(DataManagement::getInstance()->refereesMap.size()));

}

void MyCoachMainWindow::clean()
{
    ui->raceTable->setRowCount(0);

    for (QString tag : DataManagement::getInstance()->refereesMap.keys()) {
        DataManagement::getInstance()->refereesMap.value(tag)->clean();
        DataManagement::getInstance()->refereesMap.value(tag)->finished = false;
    }
}

void MyCoachMainWindow::addLapRegistry(Referee* referee)
{
    ui->raceTable->insertRow(ui->raceTable->rowCount());
    ui->raceTable->setItem(ui->raceTable->rowCount() - 1, 0, new QTableWidgetItem(QString::number(referee->laps.size())));

    if (ui->radio5x40Button->isChecked() || ui->radio60x40Button->isChecked()) {
        ui->raceTable->setItem(ui->raceTable->rowCount() - 1, 1, new QTableWidgetItem(referee->laps.at(referee->laps.size() - 1)->timeInit.toString("hh:mm:ss.zzz")));
        ui->raceTable->setItem(ui->raceTable->rowCount() - 1, 2, new QTableWidgetItem("-"));
    } else {
        ui->raceTable->setItem(ui->raceTable->rowCount() - 1, 2, new QTableWidgetItem(referee->laps.at(referee->laps.size() - 1)->timeInit.toString("hh:mm:ss.zzz")));
        //ui->raceTable->setItem(ui->raceTable->rowCount() - 1, 1, new QTableWidgetItem(referee->getTimeLastLap()));
    }
    ui->raceTable->setItem(ui->raceTable->rowCount() - 1, 3, new QTableWidgetItem(referee->name));
    ui->raceTable->setItem(ui->raceTable->rowCount() - 1, 4, new QTableWidgetItem(referee->comarca));

    //TODO: completar comprobaciones de tiempos de las demas pruebas
    if (ui->radio60x40Button->isChecked()) {
         if (!referee->isInTime(DataManagement::getInstance()->getTope6x40(referee->categoria))) {
             if (referee->estaDescalificado) {
                 for (int i = 0; i < 4; i++) {
                     ui->raceTable->item(ui->raceTable->rowCount() - 1, i)->setBackground(Qt::red);
                 }
             } else if (referee->nuevaOportunidad) {
                 for (int i = 0; i < 4; i++) {
                     ui->raceTable->item(ui->raceTable->rowCount() - 1, i)->setBackground(QBrush(QColor(248, 215, 0)));
                 }
             }
         }
    }

}

void MyCoachMainWindow::cargarBonus(QString categoria, QString filename)
{
    //DataManagement::getInstance()->bonusMap.clear();
    ExcelReader reader;
    reader.readBonus(categoria, QDir::toNativeSeparators(filename));
    mostrarMensajeCargaBonus();
}

void MyCoachMainWindow::cargarParticipantes(QString filename)
{

    DataManagement::getInstance()->refereesMap.clear();
    ExcelReader reader;
    reader.readReferees(QDir::toNativeSeparators(filename));
    updateRaceInfo();
}

bool MyCoachMainWindow::checkearFicherosPruebas(QString path)
{
    QFileInfo check_file(path);
    return check_file.exists() && check_file.isFile();
}

void MyCoachMainWindow::loadBonusFile()
{
//    QString filter = QString("Supported Files (*.xlsx)");
//    QStringList fileNames = QFileDialog::getOpenFileNames(this, tr("Select File(s)"), QDir::homePath(), filter);

//    if (fileNames.size() == 1) {
//        cargarBonus(QDir::toNativeSeparators(fileNames.at(0)));
//    }
}

void MyCoachMainWindow::create6x40Report()
{
    if (sessionFilePath == Q_NULLPTR || sessionFilePath.isEmpty()) {
        return;
    }
    SessionManagement session;
    QMap<int, QList<QTime>> map6x40 = session.loadPrueba(sessionFilePath  + "\\6x40.ses");

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

            resultado->bonificacion6x40 = DataManagement::getInstance()->getBonificacion(resultado->arbitro->categoria, C6X40, resultado->promedio.second() * 1000 + resultado->promedio.msec());
        }
        qSort(resultados.begin(), resultados.end(), MyCoachMainWindow::nombreMayorQue);

        ExcelReader reader;
        reader.create6x40Report(sessionFilePath, resultados);

        QMessageBox::information(this, "Crear informe de resultados", "Se ha creado el fichero de resultados de la prueba de 6x40 en la carpeta de trabajo correctamente.", QMessageBox::Ok);
    }
}

void MyCoachMainWindow::create2000Report()
{
    if (sessionFilePath == Q_NULLPTR || sessionFilePath.isEmpty()) {
        return;
    }
    SessionManagement session;
    QMap<int, QList<QTime>> map2000 = session.loadPrueba(sessionFilePath  + "\\2000.ses");

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
            resultado->bonificacion2000 = DataManagement::getInstance()->getBonificacion(resultado->arbitro->categoria,
                                                                                        C2000MTS, resultado->getResultado2000());
        }
        qSort(resultados.begin(), resultados.end(), MyCoachMainWindow::nombreMayorQue);

        ExcelReader reader;
        reader.create2000Report(sessionFilePath, resultados);

        QMessageBox::information(this, "Crear informe de resultados", "Se ha creado el fichero de resultados de las pruebas de 2000 metros en la carpeta de trabajo correctamente.", QMessageBox::Ok);
    }
}

void MyCoachMainWindow::createPCReport()
{
    if (sessionFilePath == Q_NULLPTR || sessionFilePath.isEmpty()) {
        return;
    }
    SessionManagement session;
    QMap<int, QList<QTime>> mapPC = session.loadPrueba(sessionFilePath  + "\\pc.ses");

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
            resultado->bonificacionPC = DataManagement::getInstance()->getBonificacion(resultado->arbitro->categoria,
                                                                                      PRUEBA_DE_CAMPO, (resultado->resultadoPC.minute() * 6000) + (resultado->resultadoPC.second() * 1000) + resultado->resultadoPC.msec());
        }
        qSort(resultados.begin(), resultados.end(), MyCoachMainWindow::nombreMayorQue);

        ExcelReader reader;
        reader.createPCReport(sessionFilePath, resultados);

        QMessageBox::information(this, "Crear informe de resultados", "Se ha creado el fichero de resultados de la prueba de campo en la carpeta de trabajo correctamente.", QMessageBox::Ok);
    }
}

void MyCoachMainWindow::createReport()
{
    if (sessionFilePath == Q_NULLPTR || sessionFilePath.isEmpty()) {
        return;
    }

    SessionManagement session;
    QMap<int, QList<QTime>> map6x40 = session.loadPrueba(sessionFilePath  + "\\6x40.ses");
    QMap<int, QList<QTime>> map2000 = session.loadPrueba(sessionFilePath  + "\\2000.ses");
    QMap<int, QList<QTime>> mapPC = session.loadPrueba(sessionFilePath  + "\\pc.ses");

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

            resultado->bonificacion6x40 = DataManagement::getInstance()->getBonificacion(resultado->arbitro->categoria,
                                                                                        C6X40, resultado->promedio.second() * 1000 + resultado->promedio.msec());
            resultado->bonificacion2000 = DataManagement::getInstance()->getBonificacion(resultado->arbitro->categoria,
                                                                                        C2000MTS, resultado->getResultado2000());


            resultado->bonificacionPC = DataManagement::getInstance()->getBonificacion(resultado->arbitro->categoria,
                                                                                      PRUEBA_DE_CAMPO, (resultado->resultadoPC.minute() * 6000) + (resultado->resultadoPC.second() * 1000) + resultado->resultadoPC.msec());
        }
        qSort(resultados.begin(), resultados.end(), MyCoachMainWindow::promedioMayorQue);

        ExcelReader reader;
        reader.createResultsReport(sessionFilePath, resultados);

        QMessageBox::information(this, "Crear informe de resultados", "Se ha creado el fichero de resultados de las pruebas en la carpeta de trabajo correctamente.", QMessageBox::Ok);
    }

}


/***MOCK**/

void MyCoachMainWindow::mockReferees()
{

}

void MyCoachMainWindow::checkRace()
{
    bool allFinished = true;
    for (Referee* referee : DataManagement::getInstance()->refereesMap.values()) {
        if (!referee->finished) {
            allFinished = false;
            break;
        }
    }

    if (allFinished) {
        stopChronometre();
    }
}

void MyCoachMainWindow::receiveTag(QString tag)
{    

    Referee* referee = DataManagement::getInstance()->refereesMap.value(tag);
    if (referee == Q_NULLPTR) {
        return;
    }
    if (referee->finished)
        return;
    QTime time = referee->chronometre.getTime();
    mutex.lock();

    if (ui->radio60x40Button->isChecked() || ui->radio5x40Button->isChecked()) {
        if (!referee->isStartLap) {
            if (!referee->isAvailableToRegister(3)) {
                mutex.unlock();
                return;
            }
        } else {
            if (!referee->isAvailableToRegister(15)) {
                mutex.unlock();
                return;
            }
        }
        referee->addLap40x6(time, DataManagement::getInstance()->getTope6x40(referee->categoria));
        if (referee->isStartLap)
            addLapRegistry(referee);
    } else {
        if (!referee->isAvailableToRegister(DataManagement::getInstance()->secondsBetweenRegister)) {
            mutex.unlock();
            return;
        }
        referee->addLap(time);
        addLapRegistry(referee);
    }

    mutex.unlock();
}

void MyCoachMainWindow::startSimulation()
{
    timerSimulation.setInterval(100);
    connect(&timerSimulation, SIGNAL(timeout()), this, SLOT(checkRace()));
    timerSimulation.start();
}
