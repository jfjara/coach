#include "dorsalestagsdialog.h"
#include "ui_dorsalestagsdialog.h"

DorsalesTagsDialog::DorsalesTagsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DorsalesTagsDialog)
{
    ui->setupUi(this);

    connect(ui->deleteButton, SIGNAL(clicked(bool)), this, SLOT(desasignarTag()));
    connect(ui->asignarButton, SIGNAL(clicked(bool)), this, SLOT(asignarTag()));
    connect(ui->guardarButton, SIGNAL(clicked(bool)), this, SLOT(guardar()));
    connect(ui->cerrarButton, SIGNAL(clicked(bool)), this, SLOT(cerrar()));
    connect(ui->leerTagButton, SIGNAL(clicked(bool)), this, SLOT(leerTarjeta()));

    QStringList tableHeader;
    tableHeader << "Dorsal" << "Tarjeta";

    ui->tableWidget->setColumnCount(2);
    ui->tableWidget->setHorizontalHeaderLabels(tableHeader);

    ui->tableWidget->horizontalHeader()->resizeSection( 0, 60 );
    ui->tableWidget->horizontalHeader()->resizeSection( 1, 400 );

    SessionManagement session;
    map = session.loadAsignacionesDorsales();

    for (int dorsal : map.keys()) {
        QString tag = map.value(dorsal);
        addDataToTable(dorsal, tag);
    }

    ui->dorsalText->setValidator(new QIntValidator(1, 1000, this));
}

DorsalesTagsDialog::~DorsalesTagsDialog()
{
    delete ui;
}

void DorsalesTagsDialog::setTag(QString tag)
{
    ui->tagText->setText(tag);
}

void DorsalesTagsDialog::addDataToTable(int dorsal, QString tag)
{
    ui->tableWidget->insertRow(ui->tableWidget->rowCount());
    ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 0, new QTableWidgetItem(QString::number(dorsal)));
    ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 1, new QTableWidgetItem(tag));
}

void DorsalesTagsDialog::asignarTag()
{
    int dorsal = ui->dorsalText->text().toUInt();
    QString tag = ui->tagText->text();

    if (map.contains(dorsal)) {
        QMessageBox::warning(this, "Error", "El dorsal que está introduciendo ya está asignado", QMessageBox::Ok);
        return;
    }

    if (map.values().contains(tag)) {
        QMessageBox::warning(this, "Error", "La tarjeta que está introduciendo ya está asignada", QMessageBox::Ok);
        return;
    }

    if (dorsal == 0) {
        QMessageBox::warning(this, "Error", "Debe introducir un dorsal válido", QMessageBox::Ok);
        return;
    }

    if (tag == Q_NULLPTR || tag.isEmpty()) {
        QMessageBox::warning(this, "Error", "Debe introducir una tarjeta válida", QMessageBox::Ok);
        return;
    }

    addDataToTable(dorsal, tag);
    ui->dorsalText->setText("");
    ui->tagText->setText("");
}

void DorsalesTagsDialog::cerrarLecturaTarjeta()
{
    delete timer;
    delete server;
    ui->leerTagButton->setEnabled(true);
}

void DorsalesTagsDialog::leerTarjeta()
{
    timer = new QTimer();
    connect(timer, SIGNAL(timeout()), this, SLOT(cerrarLecturaTarjeta()));
    timer->setInterval(5000);
    timer->start();
    server = new ServerSocket(2, Q_NULLPTR, this, Q_NULLPTR);
    server->StartServer();
    ui->leerTagButton->setEnabled(false);

}

void DorsalesTagsDialog::guardar()
{
    QMap<int, QString> asignaciones;
    QAbstractItemModel* model = ui->tableWidget->model();
    for (int row = 0; row < model->rowCount(); row++)
    {
        QModelIndex indexDorsal = model->index(row, 0 );
        QModelIndex indexTag = model->index(row, 1 );
        asignaciones.insert(indexDorsal.data().toInt(), indexTag.data().toString());
    }
    SessionManagement session;
    session.saveAsignaciones(asignaciones);

}

void DorsalesTagsDialog::cerrar()
{
    this->close();
}

void DorsalesTagsDialog::desasignarTag()
{
    QModelIndexList modelList = ui->tableWidget->selectionModel()->selectedRows();
    if (modelList.size() <= 0) {
        return;
    }
    QModelIndex index = modelList.at(0);
    QVariant tagVariant = index.model()->data(index.model()->index(index.row(), 0), Qt::DisplayRole);
    int dorsal = tagVariant.toInt();
    if (map.contains(dorsal)) {
        map.remove(dorsal);
    }
    ui->tableWidget->removeRow(index.row());
}
