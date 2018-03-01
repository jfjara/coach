#ifndef TAGGENERATORMOCK_H
#define TAGGENERATORMOCK_H

#include <QObject>
#include <QTimer>
#include <QString>
#include <QList>
#include "referee.h"
#include "datamanagement.h"

class TagGeneratorMock : public QObject
{
    Q_OBJECT
public:
    TagGeneratorMock();

    QTimer timer;
    void start();
    void stop();

public slots:
    void timeout();

signals:
    void sendTag(QString tag);

private:
    QList<QString> tags;

};

#endif // TAGGENERATORMOCK_H
