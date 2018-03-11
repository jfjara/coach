#ifndef REPORTSDIALOG_H
#define REPORTSDIALOG_H

#include <QDialog>
#include <QMessageBox>
#include <QDir>

#include "ResultadoArbitro.h"
#include "Referee.h"
#include "SessionManagement.h"
#include "ExcelReader.h"


namespace Ui {
class ReportsDialog;
}

class ReportsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ReportsDialog(QString path, bool p6x40, bool p5x40, bool p2000, bool pc, QWidget *parent = 0);
    ~ReportsDialog();

    QString path;
    bool p6x40;
    bool p5x40;
    bool p2000;
    bool pc;

    static bool promedioMayorQue(const ResultadoArbitro* a1, const ResultadoArbitro* a2) {
         return a1->getBonificacionTotal() > a2->getBonificacionTotal();
    }

    static bool nombreMayorQue(const ResultadoArbitro* a1, const ResultadoArbitro* a2) {
         return a1->arbitro->name < a2->arbitro->name;
    }

    static bool dorsalMayorQue(const ResultadoArbitro* a1, const ResultadoArbitro* a2) {
        return a1->arbitro->dorsal < a2->arbitro->dorsal;
    }

    static bool promedio6x40MayorQue(const ResultadoArbitro* a1, const ResultadoArbitro* a2) {
         return a1->bonificacion6x40 > a2->bonificacion6x40;
    }

    static bool promedio5x40MayorQue(const ResultadoArbitro* a1, const ResultadoArbitro* a2) {
         return a1->bonificacion5x40 > a2->bonificacion5x40;
    }

    static bool promedio2000MayorQue(const ResultadoArbitro* a1, const ResultadoArbitro* a2) {
         return a1->bonificacion2000 > a2->bonificacion2000;
    }

    static bool promedioPCMayorQue(const ResultadoArbitro* a1, const ResultadoArbitro* a2) {
         return a1->bonificacionPC > a2->bonificacionPC;
    }


public slots:

    void cancel();
    void createReport();

private:
    Ui::ReportsDialog *ui;

    void createReportTotal();
    void createReport6x40();
    void createReport5x40();
    void createReport2000();
    void createReportPC();

};

#endif // REPORTSDIALOG_H
