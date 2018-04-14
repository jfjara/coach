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
    tableHeader << "Vuelta" << "Tiempo vuelta" << "Tiempo total" << "Dorsal" << "Nombre" << "Categoría";
    ui->raceTable->setColumnCount(6);
    ui->raceTable->setHorizontalHeaderLabels(tableHeader);

    ui->raceTable->horizontalHeader()->resizeSection( 0, 70 );
    ui->raceTable->horizontalHeader()->resizeSection( 1, 150 );
    ui->raceTable->horizontalHeader()->resizeSection( 2, 150 );
    ui->raceTable->horizontalHeader()->resizeSection( 3, 70 );
    ui->raceTable->horizontalHeader()->resizeSection( 4, 400 );
    ui->raceTable->horizontalHeader()->resizeSection( 5, 170 );

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
    connect(&refreshTableTimer, SIGNAL(timeout()), this, SLOT(refreshTable()));
    //connect(&generatorTag, SIGNAL(sendTag(QString)), this, SLOT(receiveTag(QString)));

    ui->reportButton->setEnabled(false);

    ui->bonusButton->setVisible(false);

    configureTest6x40();

    refreshTableTimer.setInterval(10);

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

        DataManagement::getInstance()->bonusFemMap.clear();
        DataManagement::getInstance()->bonusMap.clear();

        cargarBonus("OFICIAL", path + "\\bonificaciones_oficial.xlsx");
        cargarBonus("ASISTENTE2B", path + "\\bonificaciones_asistente2b.xlsx");
        cargarBonus("3DIVISION", path + "\\bonificaciones_3division.xlsx");
        cargarBonus("ASISTENTE3DIVISION", path + "\\bonificaciones_asistente3division.xlsx");
        cargarBonus("DIVISIONHONORSENIOR", path + "\\bonificaciones_divisionhonorsenior.xlsx");
        cargarBonus("PROVINCIAL", path + "\\bonificaciones_provincial.xlsx");
        cargarBonus("NUEVOINGRESO", path + "\\bonificaciones_nuevoingreso.xlsx");
        cargarBonus("FORMADORES", path + "\\bonificaciones_formadores.xlsx");
        cargarBonus("FUTBOLSALA", path + "\\bonificaciones_futbolsala.xlsx");

        cargarBonusFem("OFICIAL", path + "\\bonificaciones_oficial_femenino.xlsx");
        cargarBonusFem("ASISTENTE2B", path + "\\bonificaciones_asistente2b_femenino.xlsx");
        cargarBonusFem("3DIVISION", path + "\\bonificaciones_3division_femenino.xlsx");
        cargarBonusFem("ASISTENTE3DIVISION", path + "\\bonificaciones_asistente3division_femenino.xlsx");
        cargarBonusFem("DIVISIONHONORSENIOR", path + "\\bonificaciones_divisionhonorsenior_femenino.xlsx");
        cargarBonusFem("PROVINCIAL", path + "\\bonificaciones_provincial_femenino.xlsx");
        cargarBonusFem("NUEVOINGRESO", path + "\\bonificaciones_nuevoingreso_femenino.xlsx");
        cargarBonusFem("FORMADORES", path + "\\bonificaciones_formadores_femenino.xlsx");
        cargarBonusFem("FUTBOLSALA", path + "\\bonificaciones_futbolsala_femenino.xlsx");

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

        ui->folderLabel->setText(sessionFilePath);

        if (!QDir(QDir::toNativeSeparators(sessionFilePath + "\\resultados\\")).exists()) {
            QDir().mkdir(QDir::toNativeSeparators(sessionFilePath + "\\resultados\\"));
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
    if (ui->radio5x40Button->isChecked() || ui->radio60x40Button->isChecked()) {
        refreshTableTimer.start();
    }

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
    //generatorTag.start();
    //startSimulation();
}

void MyCoachMainWindow::stopChronometre()
{
    enableOptions(true);
    timerSimulation.stop();
    //generatorTag.stop();
    chronometre.stop();
    refreshTableTimer.stop();

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
    //if (serverSocket != Q_NULLPTR) {
    //    delete serverSocket;
    //}
    session.save(sessionFilePath, tipoPrueba);

    if (ui->checkBox6x40->isChecked() && ui->checkBox2000->isChecked() && ui->checkBoxPC->isChecked()) {
        ui->reportButton->setEnabled(true);
    } else {
        ui->reportButton->setDisabled(false);
    }
    // clean();
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

    //limpiamos los ficheros necesarios
    SessionManagement session;
    int tipoPrueba = 0;
    if (ui->radio2000Button->isChecked()) {
        tipoPrueba = 1;
    } else if (ui->radio60x40Button->isChecked()) {
        tipoPrueba = 0;
    } else if (ui->radioPCButton->isChecked()) {
        tipoPrueba = 2;
    } else if (ui->radio5x40Button->isChecked()) {
        tipoPrueba = 3;
    }
    session.deleteFile(sessionFilePath, tipoPrueba);
}

void MyCoachMainWindow::refreshTable()
{

        mutexTabla.lock();
        int iRows = ui->raceTable->rowCount();

        for(int i = 0; i < iRows; i++) {
            QTableWidgetItem* tVuelta = ui->raceTable->item(i, 0);
            QTableWidgetItem* tDorsal = ui->raceTable->item(i, 3);
            if (tVuelta == Q_NULLPTR || tDorsal == Q_NULLPTR) {
                mutexTabla.unlock();
                return;
            }
            QString vDorsal = tDorsal->text();
            QString vVuelta = tVuelta->text();
            Referee* referee = DataManagement::getInstance()->getRefereeByDorsal(vDorsal.toInt());
            if (!referee->laps.at(vVuelta.toInt() - 1)->finalizada) {
                //hacer el set con el tiempo del chrono del arbitro
                QTime time = referee->chronometre->getTime();
                ui->raceTable->item(i, 2)->setText(time.toString("hh:mm:ss.zzz"));
            } else {
                //ui->raceTable->item(i, 2)->setText(referee->laps.last()->timeEnd.toString("hh:mm:ss.zzz"));
            }
        }
        mutexTabla.unlock();
        refreshTableTimer.start();

}

void MyCoachMainWindow::cambiarColorFila(int dorsal, int vuelta, QBrush color)
{
    mutexTabla.lock();
    int iRows = ui->raceTable->rowCount();
    for(int fila = 0; fila < iRows; fila++) {

        QTableWidgetItem* tVuelta = ui->raceTable->item(fila, 0);
        QTableWidgetItem* tDorsal = ui->raceTable->item(fila, 3);
        if (tVuelta == Q_NULLPTR || tDorsal == Q_NULLPTR) {
            mutexTabla.unlock();
            return;
        }
        QString vDorsal = tDorsal->text();
        QString vVuelta = tVuelta->text();

        if (dorsal == vDorsal.toInt() && vuelta == vVuelta.toInt()) {
            for (int i = 0; i < 6; i++) {
                ui->raceTable->item(fila, i)->setBackground(color);
            }
            Referee* referee = DataManagement::getInstance()->getRefereeByDorsal(vDorsal.toInt());
            ui->raceTable->item(fila, 2)->setText(referee->laps.last()->timeEnd.toString("hh:mm:ss.zzz"));
            mutexTabla.unlock();
            return;
        }
    }
    mutexTabla.unlock();
}

void MyCoachMainWindow::addLapRegistry(Referee* referee)
{
    mutexTabla.lock();
    //ui->raceTable->insertRow(ui->raceTable->rowCount());
     ui->raceTable->insertRow(0);
    if (ui->radio2000Button->isChecked() || ui->radioPCButton->isChecked()) {
        ui->raceTable->setItem(0, 0, new QTableWidgetItem(QString::number(referee->laps.size() - 1)));
    } else {
        ui->raceTable->setItem(0, 0, new QTableWidgetItem(QString::number(referee->laps.size())));
    }
    if (ui->radio5x40Button->isChecked() || ui->radio60x40Button->isChecked()) {
        ui->raceTable->setItem(0, 1, new QTableWidgetItem(referee->laps.at(referee->laps.size() - 1)->timeEnd.toString("hh:mm:ss.zzz")));
        ui->raceTable->setItem(0, 2, new QTableWidgetItem("-"));
    } else {
        ui->raceTable->setItem(0, 2, new QTableWidgetItem(referee->laps.at(referee->laps.size() - 1)->timeInit.toString("hh:mm:ss.zzz")));
        ui->raceTable->setItem(0, 1, new QTableWidgetItem(""));
    }
    ui->raceTable->setItem(0, 3, new QTableWidgetItem(QString::number(referee->dorsal)));
    ui->raceTable->setItem(0, 4, new QTableWidgetItem(referee->name));
    ui->raceTable->setItem(0, 5, new QTableWidgetItem(referee->categoria));
    mutexTabla.unlock();
}

void MyCoachMainWindow::cargarBonusFem(QString categoria, QString filename)
{
    //DataManagement::getInstance()->bonusMap.clear();
    ExcelReader reader;
    reader.readBonus(categoria, "FEMENINO", QDir::toNativeSeparators(filename));
    mostrarMensajeCargaBonus();
}

void MyCoachMainWindow::cargarBonus(QString categoria, QString filename)
{
    //DataManagement::getInstance()->bonusMap.clear();
    ExcelReader reader;
    reader.readBonus(categoria, "MASCULINO", QDir::toNativeSeparators(filename));
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

}

void MyCoachMainWindow::create2000Report()
{

}

void MyCoachMainWindow::createPCReport()
{

}

void MyCoachMainWindow::createReport()
{
    ReportsDialog dialog(sessionFilePath, ui->checkBox6x40->isChecked(), ui->checkBox5x40->isChecked(),
                         ui->checkBox2000->isChecked(), ui->checkBoxPC->isChecked());
    dialog.exec();
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

//void MyCoachMainWindow::receiveTag(QString tag)
//{

//    Referee* referee = DataManagement::getInstance()->refereesMap.value(tag);
//    if (referee == Q_NULLPTR) {
//        return;
//    }
//    if (referee->finished)
//        return;
//    QTime time = referee->chronometre.getTime();
//    mutex.lock();

//    if (ui->radio60x40Button->isChecked() || ui->radio5x40Button->isChecked()) {
//        if (!referee->isStartLap) {
//            if (!referee->isAvailableToRegister(3)) {
//                mutex.unlock();
//                return;
//            }
//        } else {
//            if (!referee->isAvailableToRegister(60)) {
//                mutex.unlock();
//                return;
//            }
//        }
//        referee->addLap40x6(time, DataManagement::getInstance()->getTope6x40(referee->categoria));
//        if (referee->isStartLap)
//            addLapRegistry(referee);
//    } else {
//        if (!referee->isAvailableToRegister(DataManagement::getInstance()->secondsBetweenRegister)) {
//            mutex.unlock();
//            return;
//        }
//        referee->addLap(time);
//        addLapRegistry(referee);
//    }

//    mutex.unlock();
//}

void MyCoachMainWindow::receiveTag(QString tag)
{

    Referee* referee = DataManagement::getInstance()->refereesMap.value(tag);
    if (referee == Q_NULLPTR) {
        return;
    }
    if (referee->finished)
        return;
    QTime time = referee->chronometre->getTime();
    mutex.lock();

    if (ui->radio60x40Button->isChecked() || ui->radio5x40Button->isChecked()) {
        if (referee->isStartLap) {
            if (!referee->isAvailableToRegister(80) && referee->laps.size() > 1) {
                mutex.unlock();
                return;
            }
            referee->addLap40x6(time, DataManagement::getInstance()->getTope6x40(referee->categoria, referee->gender));
            addLapRegistry(referee);
        } else {
            if (!referee->isAvailableToRegister(4)) {
                mutex.unlock();
                return;
            }
            referee->laps.last()->timeInit = time;
            referee->laps.last()->timeEnd = time;
            referee->laps.last()->finalizada = true;
            referee->lastRegister = QTime::currentTime();
            referee->isStartLap = true;
            referee->chronometre->stop();

            referee->checkFueraTiempo(DataManagement::getInstance()->getTope6x40(referee->categoria, referee->gender));

            if (referee->estaDescalificado) {
                //cambio de color
                cambiarColorFila(referee->dorsal, referee->laps.size(), Qt::red);
            }
            referee->checkFinished(6);
            if (referee->nuevaOportunidad && !referee->pintadaNuevaOportunidad) {
                //cambio de color!
                referee->pintadaNuevaOportunidad = true;
                cambiarColorFila(referee->dorsal, referee->laps.size(), QBrush(QColor(248, 215, 0)));
            }

        }

    } else {
        if (!referee->isAvailableToRegister(DataManagement::getInstance()->secondsBetweenRegister)) {
            mutex.unlock();
            return;
        }

        if (ui->radio2000Button->isChecked()) {
            referee->addLap(time);
        } else {
            referee->addLapPC(time);
        }
        addLapRegistry(referee);

        if (referee->finished && ui->radio2000Button->isChecked()) {
            if (!referee->isInTime(DataManagement::getInstance()->getTope2000(referee->categoria, referee->gender))) {
                cambiarColorFila(referee->dorsal, referee->laps.size() - 1, Qt::red);
            }
        }

        if (ui->radioPCButton->isChecked()) {
            referee->checkFueraTiempo(DataManagement::getInstance()->getTopePC(referee->categoria, referee->gender));
            if (referee->estaDescalificado) {
                //cambio de color
                cambiarColorFila(referee->dorsal, referee->laps.size() - 1, Qt::red);
            }
            if (!referee->nuevaOportunidad) {
                referee->checkFinished(2); //se esta marcsndo la salida, por eso una de mas
            } else {
                referee->checkFinished(4); //se esta marcsndo la salida, por eso una de mas
            }
            if (referee->nuevaOportunidad && !referee->pintadaNuevaOportunidad) {
                //cambio de color!
                referee->pintadaNuevaOportunidad = true;
                referee->totalLapsRace += 2;
                referee->finished = false;
                referee->isStartLap = true;
                //referee->chronometre->start();
                cambiarColorFila(referee->dorsal, referee->laps.size() - 1, QBrush(QColor(248, 215, 0)));

            }
        }

    }

    mutex.unlock();
}


void MyCoachMainWindow::startSimulation()
{
    timerSimulation.setInterval(100);
    connect(&timerSimulation, SIGNAL(timeout()), this, SLOT(checkRace()));
    timerSimulation.start();
}
