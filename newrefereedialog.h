#ifndef NEWREFEREEDIALOG_H
#define NEWREFEREEDIALOG_H

#include <QDialog>

namespace Ui {
class NewRefereeDialog;
}

class NewRefereeDialog : public QDialog
{
    Q_OBJECT

public:
    explicit NewRefereeDialog(QWidget *parent = 0);
    ~NewRefereeDialog();

    QString name;
    QString dni;
    int age;
    bool acepted = false;

public slots:
    void acceptRefeere();
    void cancel();
private:
    Ui::NewRefereeDialog *ui;
};

#endif // NEWREFEREEDIALOG_H
