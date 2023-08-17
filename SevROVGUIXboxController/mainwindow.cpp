#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    jsController = new SevROVXboxController();

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
        jsController->isRunning = true;
        jsController->start();
        ui->btnJoystick->setText("ОТКЛЮЧИТЬСЯ ОТ ДЖОЙСТИКА");
        jsConnected = true;

        controlTimer->start(100);
    }
    else
    {
        // TODO: Как правильно завершить поток?
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
    SevROVLibrary::XboxToControlData(xbox, &controlData);

    ui->edHorizontalVectorX->setText(QString::number(controlData.getHorizontalVectorX()));
    ui->edHorizontalVectorY->setText(QString::number(controlData.getHorizontalVectorY()));
    ui->edAngularVelocityZ->setText(QString::number(controlData.getAngularVelocityZ()));
    ui->edVericalThrust->setText(QString::number(controlData.getVericalThrust()));
    ui->edDepth->setText(QString::number(controlData.getDepth()));
    ui->edManipulatorState->setText(QString::number(controlData.getManipulatorState()));
    ui->edManipulatorRotate->setText(QString::number(controlData.getManipulatorRotate()));
    ui->edCameraRotate->setText(QString::number(controlData.getCameraRotate()));
    ui->edResetInitialization->setText(QString::number(controlData.getResetInitialization()));
    ui->edLightsState->setText(QString::number(controlData.getLightsState()));
}
