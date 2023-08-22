#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QScreen>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Фиксируем размер окна и убираем иконку ресайза
    setFixedSize(QSize(522, 456));
    statusBar()->setSizeGripEnabled(false);
    // Центрируем окно в пределах экрана
    move(screen()->geometry().center() - frameGeometry().center());
    setWindowTitle("ТНПА симулятор");

    // Сервер должен уметь читать сигналы управления и писать телеметрию в ответ
    rovConnector.setMode(SevROVConnector::Mode::CONTROL_READ |
                         SevROVConnector::Mode::TELEMETRY_WRITE);

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
    // Для сервера используем вариант с bind() для уставноки соединения
    if (rovConnector.getIsConnected())
    {
        // Прекращаем прослушку IP и порта
        rovConnector.closeConnection();
    }
    else
    {
        // Начинаем прослушку заданного IP и порта
        rovConnector.openConnection(ui->edIP->text(),
                                    ui->edPort->text().toInt());
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

        // Имитируем изменение телеметрии
        rovConnector.telemetry.setRoll(QRandomGenerator::global()->bounded(256));
        rovConnector.telemetry.setPitch(QRandomGenerator::global()->bounded(256));
        rovConnector.telemetry.setYaw(QRandomGenerator::global()->bounded(256));
        rovConnector.telemetry.setHeading(QRandomGenerator::global()->bounded(256));
        rovConnector.telemetry.setDepth(QRandomGenerator::global()->bounded(256));

        ui->edRoll->setText(QString::number(rovConnector.telemetry.getRoll(), 'f', 2));
        ui->edPitch->setText(QString::number(rovConnector.telemetry.getPitch(), 'f', 2));
        ui->edYaw->setText(QString::number(rovConnector.telemetry.getYaw(), 'f', 2));
        ui->edHeading->setText(QString::number(rovConnector.telemetry.getHeading(), 'f', 2));
        ui->edDepthAUV->setText(QString::number(rovConnector.telemetry.getDepth(), 'f', 2));
    }
}

