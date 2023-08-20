#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "sevrovconnector.h"
#include <QRandomGenerator>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButtonConnect_clicked();

public slots:
    void OnSocketProcessControlDatagram();
    void OnSocketConnect();
    void OnSocketDisconnect();

private:
    Ui::MainWindow *ui;
    SevROVConnector rovConnector;
};
#endif // MAINWINDOW_H
