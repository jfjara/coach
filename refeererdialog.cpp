#include "refeererdialog.h"
#include "ui_refeererdialog.h"

RefeererDialog::RefeererDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::RefeererDialog)
{
    QStringList tableHeader;
    tableHeader << "Dorsal" << "Nombre" << "Sexo" << "Comarca" << "Tag";
    ui->setupUi(this);
    ui->refeererTable->setColumnCount(5);
    ui->refeererTable->setHorizontalHeaderLabels(tableHeader);

    ui->refeererTable->horizontalHeader()->resizeSection( 0, 60 );
    ui->refeererTable->horizontalHeader()->resizeSection( 1, 250 );
    ui->refeererTable->horizontalHeader()->resizeSection( 2, 100 );
    ui->refeererTable->horizontalHeader()->resizeSection( 3, 250 );
    ui->refeererTable->horizontalHeader()->resizeSection( 4, 60 );

    connect(ui->addButton, SIGNAL(clicked(bool)), this, SLOT(addRefeerer()));
    connect(ui->deleteButton, SIGNAL(clicked(bool)), this, SLOT(removeRefeerer()));
    connect(ui->excelButton, SIGNAL(clicked(bool)), this, SLOT(loadExcel()));
    loadReferees();

    ui->addButton->setVisible(false);
}

RefeererDialog::~RefeererDialog()
{
    delete ui;
}

void RefeererDialog::loadExcel()
{
    QString filter = QString("Supported Files (*.xlsx)");
    QStringList fileNames = QFileDialog::getOpenFileNames(this, tr("Select File(s)"), QDir::homePath(), filter);

    if (fileNames.size() == 1) {
        DataManagement::getInstance()->refereesMap.clear();
        ExcelReader reader;
        reader.readReferees(QDir::toNativeSeparators(fileNames.at(0)));
        loadReferees();
    }
}

void RefeererDialog::loadReferees()
{

    for (Referee* e : DataManagement::getInstance()->refereesMap.values()) {
        insertRefereeInTable(e);
    }
}

void RefeererDialog::insertRefereeInTable(Referee* referee)
{
    ui->refeererTable->insertRow(ui->refeererTable->rowCount());
    ui->refeererTable->setItem(ui->refeererTable->rowCount() - 1, 0, new QTableWidgetItem(QString::number(referee->dorsal)));
    ui->refeererTable->setItem(ui->refeererTable->rowCount() - 1, 1, new QTableWidgetItem(referee->name));
    ui->refeererTable->setItem(ui->refeererTable->rowCount() - 1, 2, new QTableWidgetItem(referee->gender));
    ui->refeererTable->setItem(ui->refeererTable->rowCount() - 1, 3, new QTableWidgetItem(referee->comarca));
    ui->refeererTable->setItem(ui->refeererTable->rowCount() - 1, 4, new QTableWidgetItem(referee->tag));
}

void RefeererDialog::addRefeerer()
{
    NewRefereeDialog dialog;
    dialog.exec();
    if (!dialog.acepted) {
        return;
    }

    Referee* newReferee = new Referee();
    newReferee->dni = dialog.dni;
    newReferee->name = dialog.name;
    newReferee->age = dialog.age;
    DataManagement::getInstance()->addReferee(newReferee, "");
    insertRefereeInTable(newReferee);
}

void RefeererDialog::removeRefeerer()
{
    QModelIndexList modelList = ui->refeererTable->selectionModel()->selectedRows();
    if (modelList.size() <= 0) {
        return;
    }
    QModelIndex index = modelList.at(0);
    QVariant tagVariant = index.model()->data(index.model()->index(index.row(), 4), Qt::DisplayRole);
    //index.row(), 4

    //QAbstractItemModel* model = index.model();
    //model->data(model->index(row, col), Qt::DisplayRole);

    ui->refeererTable->removeRow(index.row());
    DataManagement::getInstance()->removeReferee(tagVariant.toString());
}
