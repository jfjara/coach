#ifndef BONUSDIALOG_H
#define BONUSDIALOG_H

#include <QDialog>
#include <QList>
#include "bonus.h"
#include "datamanagement.h"

namespace Ui {
class BonusDialog;
}

class BonusDialog : public QDialog
{
    Q_OBJECT

public:
    explicit BonusDialog(QWidget *parent = 0);
    ~BonusDialog();

    void cargarDatos();

public slots:
    void closeDialog();

    void cargarCategoria(QString categoria);

private:
    Ui::BonusDialog *ui;

    void insertListBonusInTable(SPORT_TEST type, QList<Bonus*> bonus);
    void insertBonusInTable(SPORT_TEST type, Bonus* bonus);
};

#endif // BONUSDIALOG_H
