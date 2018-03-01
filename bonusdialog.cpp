#include "bonusdialog.h"
#include "ui_bonusdialog.h"

BonusDialog::BonusDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::BonusDialog)
{
    ui->setupUi(this);

    connect(ui->okButton, SIGNAL(clicked(bool)), this, SLOT(closeDialog()));
    connect(ui->categoriasCombo, SIGNAL(currentTextChanged(QString)), this, SLOT(cargarCategoria(QString)));

    QStringList tableHeader6x40;
    tableHeader6x40 << "Rango inicio" << "Rango fin" << "Puntos";
    ui->_60X40Table->setColumnCount(3);
    ui->_60X40Table->setHorizontalHeaderLabels(tableHeader6x40);

    QStringList tableHeader2000;
    tableHeader2000 << "Rango inicio" << "Rango fin" << "Puntos";
    ui->_2000Table->setColumnCount(3);
    ui->_2000Table->setHorizontalHeaderLabels(tableHeader2000);

    QStringList tableHeaderPC;
    tableHeaderPC << "Rango inicio" << "Rango fin" << "Puntos";
    ui->_PCTable->setColumnCount(3);
    ui->_PCTable->setHorizontalHeaderLabels(tableHeaderPC);

    cargarDatos();
}

BonusDialog::~BonusDialog()
{
    delete ui;
}

void BonusDialog::cargarCategoria(QString categoria)
{
    QMap<SPORT_TEST, QList<Bonus*>> map = DataManagement::getInstance()->bonusMap.value(categoria);
    insertListBonusInTable(C6X40, map.value(C6X40));
    insertListBonusInTable(C2000MTS, map.value(C2000MTS));
    insertListBonusInTable(PRUEBA_DE_CAMPO, map.value(PRUEBA_DE_CAMPO));
}

void BonusDialog::cargarDatos()
{
    for (QString categoria : DataManagement::getInstance()->bonusMap.keys()) {
        ui->categoriasCombo->addItem(categoria);
    }


//    insertListBonusInTable(C6X40, DataManagement::getInstance()->bonusMap.value(C6X40));
//    insertListBonusInTable(C2000MTS, DataManagement::getInstance()->bonusMap.value(C2000MTS));
//    insertListBonusInTable(PRUEBA_DE_CAMPO, DataManagement::getInstance()->bonusMap.value(PRUEBA_DE_CAMPO));
}

void BonusDialog::insertListBonusInTable(SPORT_TEST type, QList<Bonus*> bonusList)
{
    for (Bonus* bonus : bonusList) {
        insertBonusInTable(type, bonus);
    }
}

void BonusDialog::insertBonusInTable(SPORT_TEST type, Bonus* bonus)
{
    if (type == C6X40) {
        ui->_60X40Table->insertRow(ui->_60X40Table->rowCount());
        ui->_60X40Table->setItem(ui->_60X40Table->rowCount() - 1, 0, new QTableWidgetItem(QString::number(bonus->timeInit)));
        ui->_60X40Table->setItem(ui->_60X40Table->rowCount() - 1, 1, new QTableWidgetItem(QString::number(bonus->timeEnd)));
        ui->_60X40Table->setItem(ui->_60X40Table->rowCount() - 1, 2, new QTableWidgetItem(QString::number(bonus->points)));
    } else if (type == C2000MTS) {
        ui->_2000Table->insertRow(ui->_2000Table->rowCount());
        ui->_2000Table->setItem(ui->_2000Table->rowCount() - 1, 0, new QTableWidgetItem(QString::number(bonus->timeInit)));
        ui->_2000Table->setItem(ui->_2000Table->rowCount() - 1, 1, new QTableWidgetItem(QString::number(bonus->timeEnd)));
        ui->_2000Table->setItem(ui->_2000Table->rowCount() - 1, 2, new QTableWidgetItem(QString::number(bonus->points)));
    } else if (type == PRUEBA_DE_CAMPO) {
        ui->_PCTable->insertRow(ui->_PCTable->rowCount());
        ui->_PCTable->setItem(ui->_PCTable->rowCount() - 1, 0, new QTableWidgetItem(QString::number(bonus->timeInit)));
        ui->_PCTable->setItem(ui->_PCTable->rowCount() - 1, 1, new QTableWidgetItem(QString::number(bonus->timeEnd)));
        ui->_PCTable->setItem(ui->_PCTable->rowCount() - 1, 2, new QTableWidgetItem(QString::number(bonus->points)));
    }
}

void BonusDialog::closeDialog()
{
    this->close();
}
