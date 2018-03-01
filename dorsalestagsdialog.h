#ifndef DORSALESTAGSDIALOG_H
#define DORSALESTAGSDIALOG_H

#include <QDialog>
#include <QTimer>
#include <QAbstractItemModel>
#include <QMessageBox>
#include "sessionmanagement.h"
#include "serversocket.h"

namespace Ui {
class DorsalesTagsDialog;
}

class DorsalesTagsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit DorsalesTagsDialog(QWidget *parent = 0);
    ~DorsalesTagsDialog();

    ServerSocket* server;
    QTimer* timer;

    void addDataToTable(int dorsal, QString tag);
    void setTag(QString tag);

public slots:

    void asignarTag();
    void desasignarTag();
    void leerTarjeta();
    void guardar();
    void cerrar();
    void cerrarLecturaTarjeta();

private:
    Ui::DorsalesTagsDialog *ui;

    QMap<int, QString> map;
};

#endif // DORSALESTAGSDIALOG_H
