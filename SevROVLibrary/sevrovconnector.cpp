#include "sevrovconnector.h"

SevROVConnector::SevROVConnector()
{
    // Неопределенный режим функционирования
    setMode(SevROVConnector::Mode::UNDEFINED);

    // Инициализация управления
    control.Initialize();
    // Инициализация телеметрии
    telemetry.Initialize();

    connect(&udpSocket, SIGNAL(readyRead()), this, SLOT(processDatagram()));
    // TODO: На данный момент события сокета connected/disconnected
    // не вызываются. Для этого нужен connectToHost.
    // Но тогда проблема с пакетами...
    connect(&udpSocket, SIGNAL(connected()), this, SLOT(onConnected()));
    connect(&udpSocket, SIGNAL(disconnected()), this, SLOT(onDisconnected()));
}

void SevROVConnector::setIP(QString ip)
{
    this->ip = ip;
    this->host = QHostAddress(ip);
}

void SevROVConnector::setPort(int port)
{
    this->port = port;
}

void SevROVConnector::writeTelemetryDatagram()
{
    // Проверяем, разрешено ли коннектору писать телеметрию
    if ((mode & SevROVConnector::Mode::TELEMETRY_WRITE)
        == SevROVConnector::Mode::TELEMETRY_WRITE)
    {
        qint64 bytes = udpSocket.writeDatagram(telemetry.toByteArray(), host, port);
        qDebug() << "[" << QDateTime::currentMSecsSinceEpoch()
                 << "] Telemetry datagram:" << host.toString().toStdString().c_str()
                 << ":" << port << "- packet size:" << bytes << "[bytes]";
    }
}

void SevROVConnector::writeTelemetryDatagram(QHostAddress _host, int _port)
{
    // Проверяем, разрешено ли коннектору писать телеметрию
    if ((mode & SevROVConnector::Mode::TELEMETRY_WRITE)
        == SevROVConnector::Mode::TELEMETRY_WRITE)
    {
        qint64 bytes = udpSocket.writeDatagram(telemetry.toByteArray(), _host, _port);
        qDebug() << "[" << QDateTime::currentMSecsSinceEpoch()
                 << "] Telemetry datagram:" << _host.toString().toStdString().c_str()
                 << ":" << _port << "- packet size:" << bytes << "[bytes]";
    }
}

void SevROVConnector::writeControlDatagram()
{
    // Проверяем, разрешено ли коннектору писать телеметрию
    if ((mode & SevROVConnector::Mode::CONTROL_WRITE)
        == SevROVConnector::Mode::CONTROL_WRITE)
    {
        qint64 bytes = udpSocket.writeDatagram(control.toByteArray(), host, port);
        qDebug() << "[" << QDateTime::currentMSecsSinceEpoch()
                 << "] Control datagram:" << host.toString().toStdString().c_str()
                 << ":" << port << "- packet size:" << bytes << "[bytes]";
    }
}

void SevROVConnector::writeControlDatagram(QHostAddress _host, int _port)
{
    // Проверяем, разрешено ли коннектору писать телеметрию
    if ((mode & SevROVConnector::Mode::CONTROL_WRITE)
        == SevROVConnector::Mode::CONTROL_WRITE)
    {
        qint64 bytes = udpSocket.writeDatagram(control.toByteArray(), _host, _port);
        qDebug() << "[" << QDateTime::currentMSecsSinceEpoch()
                 << "] Control datagram:" << _host.toString().toStdString().c_str()
                 << ":" << _port << "- packet size:" << bytes << "[bytes]";
    }
}

void SevROVConnector::processDatagram()
{
    // Размер пакета с телеметрией = 20 байт
    if (udpSocket.pendingDatagramSize() == 20)
    {
        // Проверяем, разрешено ли коннектору читать телеметрию
        if ((mode & SevROVConnector::Mode::TELEMETRY_READ)
            == SevROVConnector::Mode::TELEMETRY_READ)
        {
            QByteArray datagram;
            do {
                datagram.resize(udpSocket.pendingDatagramSize());
                udpSocket.readDatagram(datagram.data(), datagram.size());
            } while (udpSocket.hasPendingDatagrams());

            QDataStream in(&datagram, QIODevice::ReadOnly);
            in.setFloatingPointPrecision(QDataStream::SinglePrecision);
            in.setVersion(QDataStream::Qt_6_3);

            float Roll;
            float Pitch;
            float Yaw;
            float Heading;
            float Depth;

            in >> Roll;
            in >> Pitch;
            in >> Yaw;
            in >> Heading;
            in >> Depth;

            telemetry.setRoll(Roll);
            telemetry.setPitch(Pitch);
            telemetry.setYaw(Yaw);
            telemetry.setHeading(Heading);
            telemetry.setDepth(Depth);

            emit OnProcessTelemetryDatagram();
        }
    }

    // Размер пакета с управлением = 34 байт
    if (udpSocket.pendingDatagramSize() == 34)
    {
        // Проверяем, разрешено ли коннектору читать телеметрию
        if ((mode & SevROVConnector::Mode::CONTROL_READ)
            == SevROVConnector::Mode::CONTROL_READ)
        {
            QByteArray datagram;
            QHostAddress senderAddress = QHostAddress::Null;
            quint16 senderPort = 0;

            do {
                datagram.resize(udpSocket.pendingDatagramSize());
                udpSocket.readDatagram(datagram.data(), datagram.size(),
                                       &senderAddress, &senderPort);
            } while (udpSocket.hasPendingDatagrams());

            QDataStream in(&datagram, QIODevice::ReadOnly);
            in.setFloatingPointPrecision(QDataStream::SinglePrecision);
            in.setVersion(QDataStream::Qt_6_3);

            float HorizontalVectorX;
            float HorizontalVectorY;
            float VericalThrust;
            float Depth;
            float AngularVelocityZ;
            float ManipulatorState;
            float ManipulatorRotate;
            float CameraRotate;
            int8_t ResetInitialization;
            int8_t LightsState;

            in >> HorizontalVectorX;
            in >> HorizontalVectorY;
            in >> VericalThrust;
            in >> Depth;
            in >> AngularVelocityZ;
            in >> ManipulatorState;
            in >> ManipulatorRotate;
            in >> CameraRotate;
            in >> ResetInitialization;
            in >> LightsState;

            control.setHorizontalVectorX(HorizontalVectorX);
            control.setHorizontalVectorY(HorizontalVectorY);
            control.setVericalThrust(VericalThrust);
            control.setDepth(Depth);
            control.setAngularVelocityZ(AngularVelocityZ);
            control.setManipulatorState(ManipulatorState);
            control.setManipulatorRotate(ManipulatorRotate);
            control.setCameraRotate(CameraRotate);
            control.setResetInitialization(ResetInitialization);
            control.setLightsState(LightsState);

            emit OnProcessControlDatagram();

            // Если коннектор должен ответить на датаграмму -
            // отправляем телеметрию
            if ((mode & SevROVConnector::Mode::TELEMETRY_WRITE)
                == SevROVConnector::Mode::TELEMETRY_WRITE)
            {
                if ((senderAddress != QHostAddress::Null) && (senderPort != 0))
                    writeTelemetryDatagram(senderAddress, senderPort);
            }
        }
    }
}

// !!! Должно использоваться только для коннектора в режиме сервера !!!
void SevROVConnector::openConnection()
{
    // Открываем канал для входящих сообщений
    bool result = udpSocket.bind(host, port);

    if (result)
        onConnected();
    else
        onDisconnected();
}
// !!! Должно использоваться только для коннектора в режиме сервера !!!
void SevROVConnector::openConnection(int port)
{
    this->ip = "127.0.0.1";
    this->host = QHostAddress(ip);
    this->port = port;

    bool result = udpSocket.bind(host, port);

    if (result)
        onConnected();
    else
        onDisconnected();
}

// !!! Должно использоваться только для коннектора в режиме сервера !!!
void SevROVConnector::openConnection(QString ip, int port)
{    
    this->ip = ip;
    this->host = QHostAddress(ip);
    this->port = port;

    bool result = udpSocket.bind(host, port);

    if (result)
        onConnected();
    else
        onDisconnected();
}
// !!! Должно использоваться только для коннектора в режиме сервера !!!
void SevROVConnector::closeConnection()
{
    udpSocket.close();
}

void SevROVConnector::onConnected()
{
    isConnected = true;
    emit OnConnected();
}

void SevROVConnector::onDisconnected()
{
    isConnected = false;
    emit OnDisconnected();
}

bool SevROVConnector::getIsConnected()
{
    return isConnected;
}

void SevROVConnector::setMode(std::uint8_t mode)
{
    this->mode = mode;
}

std::uint8_t SevROVConnector::getMode()
{
    return mode;
}

void SevROVConnector::connectToHost(QString ip, int port)
{
    udpSocket.connectToHost(QHostAddress(ip), port, QIODeviceBase::ReadWrite);
}
void SevROVConnector::disconnectFromHost()
{
    udpSocket.disconnectFromHost();
}
