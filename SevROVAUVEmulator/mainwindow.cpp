#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    rovConnector.setMode(SevROVConnector::Mode::CONTROL_READ);

    connect(&rovConnector, SIGNAL(OnConnected()), this, SLOT(OnSocketConnect()));
    connect(&rovConnector, SIGNAL(OnDisconnected()), this, SLOT(OnSocketDisconnect()));
    connect(&rovConnector, SIGNAL(OnProcessControlDatagram()), this, SLOT(OnSocketProcessControlDatagram()));
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pushButtonConnect_clicked()
{
    if (rovConnector.getIsConnected())
    {
        rovConnector.closeConnection();
    }
    else
    {
        rovConnector.openConnection(ui->edIP->text(),
                                    ui->edPortIn->text().toInt(),
                                    ui->edPortOut->text().toInt());
    }
}

void MainWindow::OnSocketConnect()
{
    qDebug() << "OnSocketConnect()";
    ui->pushButtonConnect->setText("Разъединить");
}
void MainWindow::OnSocketDisconnect()
{
    qDebug() << "OnSocketDisconnect()";
    ui->pushButtonConnect->setText("Соединить");
}
void MainWindow::OnSocketProcessControlDatagram()
{
    qDebug() << "OnSocketProcessControlDatagram()";

    // Проверяем, разрешено ли коннектору читать телеметрию
    if ((rovConnector.getMode() & SevROVConnector::Mode::CONTROL_READ)
        == SevROVConnector::Mode::CONTROL_READ)
    {
        rovConnector.control.printDebugInfo();

        ui->edHorizontalVectorX->setText(QString::number(rovConnector.control.getHorizontalVectorX()));
        ui->edHorizontalVectorY->setText(QString::number(rovConnector.control.getHorizontalVectorY()));
        ui->edAngularVelocityZ->setText(QString::number(rovConnector.control.getAngularVelocityZ()));
        ui->edVericalThrust->setText(QString::number(rovConnector.control.getVericalThrust()));
        ui->edDepth->setText(QString::number(rovConnector.control.getDepth()));
        ui->edManipulatorState->setText(QString::number(rovConnector.control.getManipulatorState()));
        ui->edManipulatorRotate->setText(QString::number(rovConnector.control.getManipulatorRotate()));
        ui->edCameraRotate->setText(QString::number(rovConnector.control.getCameraRotate()));
        ui->edResetInitialization->setText(QString::number(rovConnector.control.getResetInitialization()));
        ui->edLightsState->setText(QString::number(rovConnector.control.getLightsState()));
    }
}

