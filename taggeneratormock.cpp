#include "taggeneratormock.h"
#include <QDebug>

TagGeneratorMock::TagGeneratorMock()
{



    //tags.append("A1");
    //tags.append("A2");
    //tags.append("A3");
    //tags.append("A4");
    //tags.append("A5");
    timer.setInterval(7000);
    connect(&timer, SIGNAL(timeout()), this, SLOT(timeout()));
}

void TagGeneratorMock::start()
{
    tags.clear();
    foreach (QString t, DataManagement::getInstance()->refereesMap.keys()) {
        tags.append(t);
    }
    timer.start();
}

void TagGeneratorMock::stop()
{
    timer.stop();
    //disconnect(&timer);
}

void TagGeneratorMock::timeout()
{
    int total = tags.size();
    //qDebug() <<"Total: " << QString::number(total);
    int pos = qrand() % total;
    timer.stop();
    timer.start();
    emit sendTag(tags.at(pos));
}


