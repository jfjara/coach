#include "newrefereedialog.h"
#include "ui_newrefereedialog.h"

NewRefereeDialog::NewRefereeDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NewRefereeDialog)
{
    ui->setupUi(this);
    connect(ui->buttonBox, SIGNAL(accepted()), this, SLOT(acceptRefeere()));
}

NewRefereeDialog::~NewRefereeDialog()
{
    delete ui;
}

void NewRefereeDialog::acceptRefeere()
{
    name = ui->nameText->text();
    dni = ui->dniText->text();
    age = ui->ageText->text().toInt();
    acepted = true;
    close();
}

void NewRefereeDialog::cancel()
{
    close();
}
