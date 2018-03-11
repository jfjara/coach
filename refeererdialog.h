#ifndef REFEERERDIALOG_H
#define REFEERERDIALOG_H

#include <QDialog>
#include <QFileDialog>
#include <QDir>
#include <QAbstractItemModel>
#include "datamanagement.h"
#include "newrefereedialog.h"
#include "excelreader.h"

#include "referee.h"

namespace Ui {
class RefeererDialog;
}

class RefeererDialog : public QDialog
{
    Q_OBJECT

public:
    explicit RefeererDialog(QWidget *parent = 0);
    ~RefeererDialog();

    static bool dorsalMayorQue(const Referee* a1, const Referee* a2) {
        return a1->dorsal < a2->dorsal;
    }

public slots:
    void addRefeerer();
    void removeRefeerer();
    void loadExcel();

private:
    Ui::RefeererDialog *ui;

    void loadReferees();

    void insertRefereeInTable(Referee* referee);
};

#endif // REFEERERDIALOG_H
