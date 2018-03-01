#include "chronowidget.h"
#include "ui_chronowidget.h"

ChronoWidget::ChronoWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ChronoWidget)
{
    ui->setupUi(this);

    connect(&t, SIGNAL(timeout()), this, SLOT(updateTime()));

}

ChronoWidget::~ChronoWidget()
{
    delete ui;

}

void ChronoWidget::start()
{    
    t.start(10);
    chrono.restart();
}

void ChronoWidget::updateTime()
{
    ui->displayLabel->setText(chrono.getTime().toString() +":" + QString::number(chrono.getTime().msec()));
}

void ChronoWidget::stop()
{
    isStarted = false;
    t.stop();
    chrono.stop();
}
