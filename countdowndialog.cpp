#include "countdowndialog.h"
#include "ui_countdowndialog.h"
#include <QDebug>

CountDownDialog::CountDownDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CountDownDialog)
{
    ui->setupUi(this);
    setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
    connect(&t, SIGNAL(timeout()), this, SLOT(updateTime()));
    t.setInterval(990);
    ui->numberLabel->setText("10");
}

CountDownDialog::~CountDownDialog()
{
    delete ui;
}

void CountDownDialog::updateTime()
{


    if (time <= 3) {
        ui->numberLabel->setStyleSheet("QLabel { color : red; }");
    }
    ui->numberLabel->setText(QString::number(time));
    this->update();

    if (time <= 0) {
        close();
        t.stop();

    }
    time--;
    QSound::play(":/sounds/sounds/beep.wav");


}

void CountDownDialog::init()
{
    time = 10;
    ui->numberLabel->setStyleSheet("QLabel { color : black; }");
    ui->numberLabel->setText("10");
}

void CountDownDialog::start()
{
    t.start();

}

