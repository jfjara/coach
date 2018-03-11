#ifndef MYCOACHMAINWINDOW_H
#define MYCOACHMAINWINDOW_H

class ServerSocket;

#include <QMainWindow>
#include <QTimer>
#include <QFileDialog>
#include <QDir>
#include <QBrush>
#include <QMessageBox>
#include <QMutex>
#include <QFileInfo>

#include "clientsocket.h"
#include "serversocket.h"
#include "chronowidget.h"

#include "datamanagement.h"
#include "csvmanagement.h"
#include "refeererdialog.h"
#include "configdialog.h"
#include "chrono.h"
#include "countdowndialog.h"
#include "connectionthread.h"
#include "workerconnection.h"
#include "taggeneratormock.h"
#include "excelreader.h"
#include "bonusdialog.h"
#include "sessionmanagement.h"
#include "dorsalestagsdialog.h"
#include "messagedialog.h"
#include "calibradordialog.h"
#include "resultadoarbitro.h"
#include "reportsdialog.h"

namespace Ui {
class MyCoachMainWindow;
}

class MyCoachMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MyCoachMainWindow(QWidget *parent = 0);
    ~MyCoachMainWindow();

   /* static bool promedioMayorQue(const ResultadoArbitro* a1, const ResultadoArbitro* a2) {
         return a1->getBonificacionTotal() > a2->getBonificacionTotal();
    }

    static bool nombreMayorQue(const ResultadoArbitro* a1, const ResultadoArbitro* a2) {
         return a1->arbitro->name > a2->arbitro->name;
    }*/


    ChronoWidget chronometre;
    int currentLap = 0;
    ClientSocket client;
    TagGeneratorMock generatorTag;

    ConnectionThread* checkConnectionThread;

    QString sessionFilePath;

    ServerSocket* serverSocket;

    QMutex mutex;
    int i = 0;

public slots:
    void startChonometre();
    void stopChronometre();
    void initRace();

    void openRefereeDialog();    
    void openConfigureDialog();
    void openDorsalesTagsDialog();
    void mostrarBonus();
    void loadBonusFile();
    void createReport();
    void create6x40Report();
    void create2000Report();
    void createPCReport();

    void updateDateTime();

    void configureTest6x40();
    void configureTest5x40();
    void configureTest2000();
    void configureTestPC();

    void checkRace();
    void clean();

    void newSession();
    void loadSession();
    void saveSession();

    void receiveTag(QString tag);
private:
    Ui::MyCoachMainWindow *ui;    
    QTimer timerStartRace;
    CountDownDialog countDownDialog;
    QTimer dateAndHourTimer;
    WorkerConnection* workerConnection;


    void updateRaceInfo();
    void addLapRegistry(Referee* referee);
    void mostrarMensajeCargaBonus();
    void enableOptions(bool enable);

    void mockReferees();
    void startSimulation();

    QTimer timerSimulation;


    void cargarBonus(QString categoria, QString path);
    void cargarBonusFem(QString categoria, QString path);

    void cargarParticipantes(QString path);
    bool checkearFicherosPruebas(QString path);


};

#endif // MYCOACHMAINWINDOW_H
