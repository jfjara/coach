#ifndef COUNTDOWNDIALOG_H
#define COUNTDOWNDIALOG_H

#include <QDialog>
#include <QTimer>

#include <QMediaPlayer>
#include <QSound>

#include "chrono.h"

namespace Ui {
class CountDownDialog;
}

class CountDownDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CountDownDialog(QWidget *parent = 0);
    ~CountDownDialog();

    void init();
    void start();
public slots:
    void updateTime();

private:
    Ui::CountDownDialog *ui;
    QTimer t;
    int time = 10;
    //Chrono chrono;
    QMediaPlayer* player;
};

#endif // COUNTDOWNDIALOG_H
