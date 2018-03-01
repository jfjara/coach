#ifndef CONFIGDIALOG_H
#define CONFIGDIALOG_H

#include <QDialog>

#include "datamanagement.h"

namespace Ui {
class ConfigDialog;
}

class ConfigDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ConfigDialog(QWidget *parent = 0);
    ~ConfigDialog();

public slots:
    void configure();

private:
    Ui::ConfigDialog *ui;
};

#endif // CONFIGDIALOG_H
