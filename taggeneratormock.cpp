#include "taggeneratormock.h"
#include <QDebug>

TagGeneratorMock::TagGeneratorMock()
{

    tags.append("000000000000000000024120");
    tags.append("000000000000000000024402");
    tags.append("000000000000000000023831");
    tags.append("000000000000000000023183");
    tags.append("AABBCCDD0000000000020726");



    /*tags.append("000000000000000000024578");
    tags.append("000000000000000000020845");
    tags.append("000000000000000000024067");
    tags.append("000000000000000000022590");
    tags.append("000000000000000000023717");
    tags.append("000000000000000000024824");
    tags.append("000000000000000000023872");
    tags.append("000000000000000000024059");
    tags.append("000000000000000000023938");
    tags.append("000000000000000000024755");
    tags.append("000000000000000000024573");
    tags.append("000000000000000000024357");
    tags.append("000000000000000000021158");
    tags.append("000000000000000000024780");*/


    int delay = qrand() % 200;
    timer.setInterval(100 + delay);
    connect(&timer, SIGNAL(timeout()), this, SLOT(timeout()));
}

void TagGeneratorMock::start()
{
    //tags.clear();
    /*foreach (QString t, DataManagement::getInstance()->refereesMap.keys()) {
        tags.append(t);
    }*/
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
    int delay = qrand() % 200;
    timer.setInterval(100 + delay);
    timer.start();
    emit sendTag(tags.at(pos));
}


