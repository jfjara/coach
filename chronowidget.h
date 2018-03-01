#ifndef CHRONOWIDGET_H
#define CHRONOWIDGET_H

#include <QWidget>
#include <QTimer>

#include "chrono.h"

namespace Ui {
class ChronoWidget;
}

class ChronoWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ChronoWidget(QWidget *parent = 0);
    ~ChronoWidget();

    QTimer t;
    Chrono chrono;
    bool isStarted = false;

    void start();
    void stop();

public slots:
    void updateTime();

private:
    Ui::ChronoWidget *ui;
};

#endif // CHRONOWIDGET_H
