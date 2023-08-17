#ifndef SEVROVCONNECTOR_H
#define SEVROVCONNECTOR_H

#include <QUdpSocket>

#include "sevrovcontroldata.h"
#include "sevrovtelemetrydata.h"

class SevROVConnector : public QObject
{
    Q_OBJECT
public:
    SevROVConnector();

    enum Mode {
        UNDEFINED               = 0,
        CONTROL_READ            = 1,
        CONTROL_WRITE           = 2,
        CONTROL_READWRITE       = 3,
        TELEMETRY_READ          = 16,
        TELEMETRY_WRITE         = 32,
        TELEMETRY_READWWRITE    = 48
    };
    void setMode(std::uint8_t mode);
    std::uint8_t getMode();

    SevROVControlData control;
    SevROVTelemetryData telemetry;

    void setIP(QString ip);
    void setPortIn(int port);
    void setPortOut(int port);
    void writeTelemetryDatagram();

    void openConnection();
    void openConnection(QString ip, int portIn, int portOut);
    void closeConnection();
    bool getIsConnected();

private:
    QUdpSocket udpSocket;

    QString ip;
    QHostAddress host;
    int portIn;     // Порт для прослушивания входящих сообщений
    int portOut;    // Порт для отпрпавки датаграм
    bool isConnected = false;

    std::uint8_t mode;

private slots:
    void processDatagram();
    void onConnected();
    void onDisconnected();

signals:
    void OnProcessDatagram();
    void OnProcessTelemetryDatagram();
    void OnProcessControlDatagram();
    void OnConnected();
    void OnDisconnected();
};

#endif // SEVROVCONNECTOR_H
