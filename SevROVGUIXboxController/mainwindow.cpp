#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Создаем объект джойстик-контроллера и получаем список доступных джойстиков
    jsController = new SevROVXboxController();
    QList<QString> jsList = jsController->GetJoystickList();
    for (int i = 0; i < jsList.count(); i++)
        ui->cbJoystickList->addItem(jsList[i]);

    ui->btnJoystick->setEnabled(jsList.count() > 0);

    // Кнопки
    connect(jsController, &SevROVXboxController::OnButtonA,
            this, &MainWindow::OnButtonA);
    connect(jsController, &SevROVXboxController::OnButtonB,
            this, &MainWindow::OnButtonB);
    connect(jsController, &SevROVXboxController::OnButtonX,
            this, &MainWindow::OnButtonX);
    connect(jsController, &SevROVXboxController::OnButtonY,
            this, &MainWindow::OnButtonY);
    connect(jsController, &SevROVXboxController::OnButtonLBumper,
            this, &MainWindow::OnButtonLBumper);
    connect(jsController, &SevROVXboxController::OnButtonRBumper,
            this, &MainWindow::OnButtonRBumper);
    connect(jsController, &SevROVXboxController::OnButtonView,
            this, &MainWindow::OnButtonView);
    connect(jsController, &SevROVXboxController::OnButtonMenu,
            this, &MainWindow::OnButtonMenu);
    connect(jsController, &SevROVXboxController::OnDPad,
            this, &MainWindow::OnDPad);

    // Оси
    connect(jsController, &SevROVXboxController::OnAxisLStickX,
            this, &MainWindow::OnAxisLStickX);
    connect(jsController, &SevROVXboxController::OnAxisLStickY,
            this, &MainWindow::OnAxisLStickY);
    connect(jsController, &SevROVXboxController::OnAxisRStickX,
            this, &MainWindow::OnAxisRStickX);
    connect(jsController, &SevROVXboxController::OnAxisRStickY,
            this, &MainWindow::OnAxisRStickY);
    connect(jsController, &SevROVXboxController::OnAxisLTrigger,
            this, &MainWindow::OnAxisLTrigger);
    connect(jsController, &SevROVXboxController::OnAxisRTrigger,
            this, &MainWindow::OnAxisRTrigger);

    // Начальные значения
    ui->edA->setText(QString::number(0));
    ui->edB->setText(QString::number(0));
    ui->edX->setText(QString::number(0));
    ui->edY->setText(QString::number(0));
    ui->edLBumper->setText(QString::number(0));
    ui->edRBumper->setText(QString::number(0));
    ui->edView->setText(QString::number(0));
    ui->edMenu->setText(QString::number(0));
    ui->edDPad->setText(QString::number(0));
    ui->edLStickX->setText(QString::number(0));
    ui->edLStickY->setText(QString::number(0));
    ui->edRStickX->setText(QString::number(0));
    ui->edRStickY->setText(QString::number(0));
    ui->edLTrigger->setText(QString::number(-32768));
    ui->edRTrigger->setText(QString::number(-32768));

    // Зеркалим данные
    xbox.A = 0;
    xbox.B = 0;
    xbox.X = 0;
    xbox.Y = 0;
    xbox.LBumper = 0;
    xbox.RBumper = 0;
    xbox.View = 0;
    xbox.Menu = 0;
    xbox.DPad = 0;
    xbox.LStickX = 0;
    xbox.LStickY = 0;
    xbox.RStickX = 0;
    xbox.RStickY = 0;
    xbox.LTrigger = -32768;
    xbox.RTrigger = -32768;

    ui->gbAxis->setEnabled(jsConnected);
    ui->gbButtons->setEnabled(jsConnected);

    controlTimer = new QTimer(this);
    connect(controlTimer, &QTimer::timeout,
            this, &MainWindow::OnControlTimer);

    // Создаем коннектор с AUV. Клиент должен уметь писать управление и читать телеметрию
    rovConnector.setMode(SevROVConnector::Mode::CONTROL_WRITE |
                         SevROVConnector::Mode::TELEMETRY_READ);


    connect(&rovConnector, SIGNAL(OnConnected()), this, SLOT(OnSocketConnect()));
    connect(&rovConnector, SIGNAL(OnDisconnected()), this, SLOT(OnSocketDisconnect()));
    connect(&rovConnector, SIGNAL(OnProcessTelemetryDatagram()), this, SLOT(OnSocketProcessTelemetryDatagram()));    

    ui->edAUVConnection->setEnabled(false);
}

MainWindow::~MainWindow()
{
    if (controlTimer->isActive())
        controlTimer->stop();

    delete controlTimer;
    delete jsController;
    delete ui;
}


void MainWindow::on_btnJoystick_clicked()
{
    if (!jsConnected)
    {
        jsController->OpenJoystick(ui->cbJoystickList->currentIndex());

        jsController->isRunning = true;
        jsController->start(); // Запуск процесса в поток
        ui->btnJoystick->setText("ОТКЛЮЧИТЬСЯ ОТ ДЖОЙСТИКА");
        jsConnected = true;

        // IP b Port будут использоваться только для записи датаграммы
        // Мы поймем, что ответ пришел, если поднимется событие OnProcessTelemetryDatagram
        rovConnector.setIP(ui->edIP->text());
        rovConnector.setPort(ui->edPort->text().toInt());

        controlTimer->start(100);
    }
    else
    {

        jsController->CloseJoystick();        
        jsController->isRunning = false;
        jsController->quit();
        ui->btnJoystick->setText("ПОДКЛЮЧИТЬСЯ К ДЖОЙСТИКУ");
        jsConnected = false;

        controlTimer->stop();
    }

    ui->gbAxis->setEnabled(jsConnected);
    ui->gbButtons->setEnabled(jsConnected);
}

void MainWindow::OnButtonA(short value)
{
    ui->edA->setText(QString::number(value));
    xbox.A = value;
}
void MainWindow::OnButtonB(short value)
{
    ui->edB->setText(QString::number(value));    
    xbox.B = value;
}
void MainWindow::OnButtonX(short value)
{
    ui->edX->setText(QString::number(value));
    xbox.X = value;
}
void MainWindow::OnButtonY(short value)
{
    ui->edY->setText(QString::number(value));
    xbox.Y = value;
}
void MainWindow::OnButtonLBumper(short value)
{
    ui->edLBumper->setText(QString::number(value));
    xbox.LBumper = value;
}
void MainWindow::OnButtonRBumper(short value)
{
    ui->edRBumper->setText(QString::number(value));
    xbox.RBumper = value;
}
void MainWindow::OnButtonView(short value)
{
    ui->edView->setText(QString::number(value));
    xbox.View = value;
}
void MainWindow::OnButtonMenu(short value)
{
    ui->edMenu->setText(QString::number(value));
    xbox.Menu = value;
}
void MainWindow::OnDPad(short value)
{
    ui->edDPad->setText(QString::number(value));
    xbox.DPad = value;
}
void MainWindow::OnAxisLStickX(short value)
{
    ui->edLStickX->setText(QString::number(value));
    xbox.LStickX = value;
}
void MainWindow::OnAxisLStickY(short value)
{
    ui->edLStickY->setText(QString::number(value));
    xbox.LStickY = value;
}
void MainWindow::OnAxisRStickX(short value)
{
    ui->edRStickX->setText(QString::number(value));
    xbox.RStickX = value;
}
void MainWindow::OnAxisRStickY(short value)
{
    ui->edRStickY->setText(QString::number(value));
    xbox.RStickY = value;
}
void MainWindow::OnAxisLTrigger(short value)
{
    ui->edLTrigger->setText(QString::number(value));
    xbox.LTrigger = value;
}
void MainWindow::OnAxisRTrigger(short value)
{
    ui->edRTrigger->setText(QString::number(value));
    xbox.RTrigger = value;
}
void MainWindow::OnControlTimer()
{
    SevROVLibrary::XboxToControlData(xbox, &rovConnector.control);

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

    // При соединении с джойстиком уже задали IP и Port,
    // которые будут использоваться для записи датаграммы
    if (rovConnector.getIsConnected())
        rovConnector.writeControlDatagram();
}

void MainWindow::on_btnAUV_clicked()
{

    // Для клиента держать постоянным открытым соедниение не нужно
    // For UDP sockets, after binding, the signal QUdpSocket::readyRead()
    // is emitted whenever a UDP datagram arrives on the specified
    // address and port. Thus, this function is useful to write UDP servers.
    // Т.е. bind нужно использовать только дла написания сервера.
    // для клиента держать соединение с сервером получается не обязательно


    //if (rovConnector.getIsConnected())
    //{
    //    rovConnector.closeConnection();
    //}
    //else
    //{
    //    // Создаем коннектор: Определяем IP и номер порта сервера
    //    rovConnector.openConnection(ui->edIP->text(),
    //                                ui->edPort->text().toInt());
    //}

    // Будем использовать connectToHost и disconnectFromHost
    if (rovConnector.getIsConnected())
    {
        rovConnector.disconnectFromHost();
    }
    else
    {
        // Создаем связь с сервером: Определяем IP и номер порта сервера
        rovConnector.connectToHost(ui->edIP->text(),
                                   ui->edPort->text().toInt());
    }

}

void MainWindow::OnSocketConnect()
{
    qDebug() << "OnSocketConnect()";
    ui->btnAUV->setText("ОТКЛЮЧИТЬСЯ ОТ ТНПА");
    ui->edAUVConnection->setText("Соединение с ТНПА установлено");
}
void MainWindow::OnSocketDisconnect()
{
    qDebug() << "OnSocketDisconnect()";
    ui->btnAUV->setText("ПОДКЛЮЧИТЬСЯ К ТНПА");
    ui->edAUVConnection->setText("Соединение с ТНПА разорвано");
}
void MainWindow::OnSocketProcessTelemetryDatagram()
{
    qDebug() << "OnSocketProcessTelemetryDatagram()";

    // Проверяем, разрешено ли коннектору читать телеметрию
    if ((rovConnector.getMode() & SevROVConnector::Mode::TELEMETRY_READ)
        == SevROVConnector::Mode::TELEMETRY_READ)
    {
        ui->edRoll->setText(QString::number(rovConnector.telemetry.getRoll(), 'f', 2));
        ui->edPitch->setText(QString::number(rovConnector.telemetry.getPitch(), 'f', 2));
        ui->edYaw->setText(QString::number(rovConnector.telemetry.getYaw(), 'f', 2));
        ui->edHeading->setText(QString::number(rovConnector.telemetry.getHeading(), 'f', 2));
        ui->edDepthAUV->setText(QString::number(rovConnector.telemetry.getDepth(), 'f', 2));

        rovConnector.telemetry.printDebugInfo();
    }
}

