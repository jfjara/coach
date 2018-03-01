#-------------------------------------------------
#
# Project created by QtCreator 2017-11-18T18:50:29
#
#-------------------------------------------------

QT       += core gui multimedia network sql axcontainer

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = MyCoach
TEMPLATE = app


DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += main.cpp\
        mycoachmainwindow.cpp \
    chronowidget.cpp \
    chrono.cpp \
    refeererdialog.cpp \
    referee.cpp \
    newrefereedialog.cpp \
    datamanagement.cpp \
    configdialog.cpp \
    race.cpp \
    lap.cpp \
    csvmanagement.cpp \
    countdowndialog.cpp \
    configmanagement.cpp \
    clientsocket.cpp \
    serversocket.cpp \
    mythread.cpp \
    connectionthread.cpp \
    workerconnection.cpp \
    taggeneratormock.cpp \
    bonus.cpp \
    excelreader.cpp \
    bonusdialog.cpp \
    sessionmanagement.cpp \
    dorsalestagsdialog.cpp \
    messagedialog.cpp \
    calibradordialog.cpp \
    resultadoarbitro.cpp

HEADERS  += mycoachmainwindow.h \
    chronowidget.h \
    chrono.h \
    refeererdialog.h \
    referee.h \
    newrefereedialog.h \
    datamanagement.h \
    configdialog.h \
    race.h \
    lap.h \
    csvmanagement.h \
    countdowndialog.h \
    configmanagement.h \
    clientsocket.h \
    serversocket.h \
    mythread.h \
    connectionthread.h \
    workerconnection.h \
    taggeneratormock.h \
    bonus.h \
    excelreader.h \
    constantes.h \
    bonusdialog.h \
    sessionmanagement.h \
    dorsalestagsdialog.h \
    messagedialog.h \
    calibradordialog.h \
    resultadoarbitro.h

FORMS    += mycoachmainwindow.ui \
    chronowidget.ui \
    refeererdialog.ui \
    newrefereedialog.ui \
    configdialog.ui \
    countdowndialog.ui \
    bonusdialog.ui \
    dorsalestagsdialog.ui \
    messagedialog.ui \
    calibradordialog.ui

RESOURCES += \
    icons.qrc
