#ifndef CALIBRADORDIALOG_H
#define CALIBRADORDIALOG_H

#include <QDialog>
#include <QTimer>
#include <QSound>

#include "serversocket.h"

namespace Ui {
class CalibradorDialog;
}

class CalibradorDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CalibradorDialog(QWidget *parent = 0);
    ~CalibradorDialog();
    void recibeDato();

public slots:
    void noRecibeDato();
    void cerrar();

private:
    Ui::CalibradorDialog *ui;

    ServerSocket* server = Q_NULLPTR;

    QTimer timer;
};

#endif // CALIBRADORDIALOG_H
