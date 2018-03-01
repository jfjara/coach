#include "configdialog.h"
#include "ui_configdialog.h"

ConfigDialog::ConfigDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ConfigDialog)
{
    ui->setupUi(this);

    connect(ui->buttonBox, SIGNAL(accepted()), this, SLOT(configure()));
}

ConfigDialog::~ConfigDialog()
{
    delete ui;
}

void ConfigDialog::configure()
{
    DataManagement::getInstance()->totalLaps = ui->totalLapSpinBox->value();
    DataManagement::getInstance()->distanceLap = ui->distanceLapSpinBox->value();
    close();
}
