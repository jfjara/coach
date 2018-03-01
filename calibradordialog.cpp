#include "calibradordialog.h"
#include "ui_calibradordialog.h"

CalibradorDialog::CalibradorDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CalibradorDialog)
{
    ui->setupUi(this);
    setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
    server = new ServerSocket(3, Q_NULLPTR, Q_NULLPTR, this);
    server->StartServer();
    timer.setInterval(200);
    connect(&timer, SIGNAL(timeout()), this, SLOT(noRecibeDato()));
    connect(ui->pushButton, SIGNAL(clicked(bool)), this, SLOT(cerrar()));
    timer.start();
}

CalibradorDialog::~CalibradorDialog()
{
    delete ui;
    if (server != Q_NULLPTR) delete server;
}

void CalibradorDialog::noRecibeDato()
{
    ui->label->setText("NO DETECTADO");
    ui->label->setStyleSheet("QLabel { color : red; }");
    timer.stop();
    timer.start();
}

void CalibradorDialog::recibeDato()
{
    ui->label->setText("DETECTADO");
    ui->label->setStyleSheet("QLabel { color : green; }");
    QSound::play(":/sounds/sounds/beep.wav");

    timer.stop();
    timer.start();
}

void CalibradorDialog::cerrar()
{
    this->close();
}
