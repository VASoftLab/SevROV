#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ctrl = new SevROVXboxController();

    // Кнопки
    connect(ctrl, &SevROVXboxController::OnButtonA,
            this, &MainWindow::OnButtonA);
    connect(ctrl, &SevROVXboxController::OnButtonB,
            this, &MainWindow::OnButtonB);
    connect(ctrl, &SevROVXboxController::OnButtonX,
            this, &MainWindow::OnButtonX);
    connect(ctrl, &SevROVXboxController::OnButtonY,
            this, &MainWindow::OnButtonY);
    connect(ctrl, &SevROVXboxController::OnButtonLBumper,
            this, &MainWindow::OnButtonLBumper);
    connect(ctrl, &SevROVXboxController::OnButtonRBumper,
            this, &MainWindow::OnButtonRBumper);
    connect(ctrl, &SevROVXboxController::OnButtonView,
            this, &MainWindow::OnButtonView);
    connect(ctrl, &SevROVXboxController::OnButtonMenu,
            this, &MainWindow::OnButtonMenu);

    // Оси
    connect(ctrl, &SevROVXboxController::OnAxisLStickX,
            this, &MainWindow::OnAxisLStickX);
    connect(ctrl, &SevROVXboxController::OnAxisLStickY,
            this, &MainWindow::OnAxisLStickY);
    connect(ctrl, &SevROVXboxController::OnAxisRStickX,
            this, &MainWindow::OnAxisRStickX);
    connect(ctrl, &SevROVXboxController::OnAxisRStickY,
            this, &MainWindow::OnAxisRStickY);
    connect(ctrl, &SevROVXboxController::OnAxisLTrigger,
            this, &MainWindow::OnAxisLTrigger);
    connect(ctrl, &SevROVXboxController::OnAxisRTrigger,
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
    ui->edLStickX->setText(QString::number(0));
    ui->edLStickY->setText(QString::number(0));
    ui->edRStickX->setText(QString::number(0));
    ui->edRStickY->setText(QString::number(0));
    ui->edLTrigger->setText(QString::number(-32768));
    ui->edRTrigger->setText(QString::number(-32768));
}

MainWindow::~MainWindow()
{
    delete ctrl;
    delete ui;
}


void MainWindow::on_btnJoystick_clicked()
{
    if (!jsConnected)
    {
        ctrl->isRunning = true;
        ctrl->start();
        ui->btnJoystick->setText("DISCONNECT");
        jsConnected = true;
    }
    else
    {
        // TODO: Как правильно завершить поток?
        ctrl->isRunning = false;
        ctrl->quit();
        ui->btnJoystick->setText("CONNECT");
        jsConnected = false;
    }
}

void MainWindow::OnButtonA(short value)
{
    ui->edA->setText(QString::number(value));
}
void MainWindow::OnButtonB(short value)
{
    ui->edB->setText(QString::number(value));
}
void MainWindow::OnButtonX(short value)
{
    ui->edX->setText(QString::number(value));
}
void MainWindow::OnButtonY(short value)
{
    ui->edY->setText(QString::number(value));
}
void MainWindow::OnButtonLBumper(short value)
{
    ui->edLBumper->setText(QString::number(value));
}
void MainWindow::OnButtonRBumper(short value)
{
    ui->edRBumper->setText(QString::number(value));
}
void MainWindow::OnButtonView(short value)
{
    ui->edView->setText(QString::number(value));
}
void MainWindow::OnButtonMenu(short value)
{
    ui->edMenu->setText(QString::number(value));
}

void MainWindow::OnAxisLStickX(short value)
{
    ui->edLStickX->setText(QString::number(value));
}
void MainWindow::OnAxisLStickY(short value)
{
    ui->edLStickY->setText(QString::number(value));
}
void MainWindow::OnAxisRStickX(short value)
{
    ui->edRStickX->setText(QString::number(value));
}
void MainWindow::OnAxisRStickY(short value)
{
    ui->edRStickY->setText(QString::number(value));
}
void MainWindow::OnAxisLTrigger(short value)
{
    ui->edLTrigger->setText(QString::number(value));
}
void MainWindow::OnAxisRTrigger(short value)
{
    ui->edRTrigger->setText(QString::number(value));
}
