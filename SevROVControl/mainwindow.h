#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QDateTime>
#include <QSettings>

#include "sevrovxboxcontroller.h"
#include "sevrovlibrary.h"
#include "sevrovconnector.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void loadSettings();
    void saveSettings();

private slots:
    void on_btnJoystick_clicked();

    void on_btnAUV_clicked();

    void on_pbPIDUpdate_clicked();

public slots:
    void OnSocketProcessTelemetryDatagram();
    void OnSocketConnect();
    void OnSocketDisconnect();

private:
    Ui::MainWindow *ui;
    QString settingsFileName;

    SevROVXboxController *jsController;
    SevROVConnector rovConnector;

    QTimer *controlTimer;

    XboxGamepad xbox;
    bool updatePID = false;

    bool jsConnected = false;

    void OnButtonA(short value);
    void OnButtonB(short value);
    void OnButtonX(short value);
    void OnButtonY(short value);
    void OnButtonLBumper(short value);
    void OnButtonRBumper(short value);
    void OnButtonView(short value);
    void OnButtonMenu(short value);
    void OnDPad(short value);

    void OnAxisLStickX(short value);
    void OnAxisLStickY(short value);
    void OnAxisRStickX(short value);
    void OnAxisRStickY(short value);
    void OnAxisLTrigger(short value);
    void OnAxisRTrigger(short value);

    void OnControlTimer();
};
#endif // MAINWINDOW_H