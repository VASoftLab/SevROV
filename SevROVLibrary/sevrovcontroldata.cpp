#include "sevrovcontroldata.h"
#include "qdebug.h"

SevROVControlData::SevROVControlData()
{
    LightsStatePrevious = QDateTime::currentDateTime();
}

void SevROVControlData::Initialize()
{
    HorizontalVectorX = 0.0;
    HorizontalVectorY = 0.0;
    VericalThrust = 0.0;
    Depth = 0.0;
    AngularVelocityZ = 0.0;
    ManipulatorState = 0.0;
    ManipulatorRotate = 0.0;
    CameraRotate = 0.0;
    ResetInitialization = 0;
    LightsState = 0;
}

void SevROVControlData::Initialize(float horizontalvectorx,
                                   float horizontalvectory,
                                   float verticalthrust,
                                   float depth,
                                   float angularVelocityz,
                                   float manipulatorstate,
                                   float manipulatorrotate,
                                   float camerarotate,
                                   int8_t resetinitialization,
                                   int8_t lightstate)
{
    HorizontalVectorX = horizontalvectorx;
    HorizontalVectorY = horizontalvectory;
    VericalThrust = verticalthrust;
    Depth = depth;
    AngularVelocityZ = angularVelocityz;
    ManipulatorState = manipulatorstate;
    ManipulatorRotate = manipulatorrotate;
    CameraRotate = camerarotate;
    ResetInitialization = resetinitialization;
    LightsState = lightstate;
}

void SevROVControlData::setHorizontalVectorX(float value)
{
    HorizontalVectorX = value;
}

void SevROVControlData::setHorizontalVectorY(float value)
{
    HorizontalVectorY = value;
}

void SevROVControlData::setVericalThrust(float value)
{
    VericalThrust = value;
}

void SevROVControlData::setDepth(float value)
{
    Depth = value;
}

void SevROVControlData::setAngularVelocityZ(float value)
{
    AngularVelocityZ = value;
}

void SevROVControlData::setManipulatorState(float value)
{
    ManipulatorState = value;
}

void SevROVControlData::setManipulatorRotate(float value)
{
    ManipulatorRotate = value;
}

void SevROVControlData::setCameraRotate(float value)
{
    CameraRotate = value;
}

void SevROVControlData::setResetInitialization(int8_t value)
{
    ResetInitialization = value;
}

void SevROVControlData::setLightsState(int8_t value)
{
    QDateTime LightsStateCurrent = QDateTime::currentDateTime();

    //qDebug() << LightsStatePrevious;
    //qDebug() << LightsStateCurrent;
    //qDebug() << LightsStatePrevious.msecsTo(LightsStateCurrent);

    // Смена режима не раньше чем раз в 5 секунд
    if (LightsStatePrevious.msecsTo(LightsStateCurrent) >= 5000)
    {
        LightsState = value;
        LightsStatePrevious = QDateTime::currentDateTime();
    }
}

float SevROVControlData::getHorizontalVectorX()
{
    return HorizontalVectorX;
}

float SevROVControlData::getHorizontalVectorY()
{
    return HorizontalVectorY;
}

float SevROVControlData::getVericalThrust()
{
    return VericalThrust;
}

float SevROVControlData::getDepth()
{
    return Depth;
}

float SevROVControlData::getAngularVelocityZ()
{
    return AngularVelocityZ;
}

float SevROVControlData::getManipulatorState()
{
    return ManipulatorState;
}

float SevROVControlData::getManipulatorRotate()
{
    return ManipulatorRotate;
}

float SevROVControlData::getCameraRotate()
{
    return CameraRotate;
}

int8_t SevROVControlData::getResetInitialization()
{
    return ResetInitialization;
}

int8_t SevROVControlData::getLightsState()
{
    return LightsState;
}

QByteArray SevROVControlData::toByteArray()
{
    QByteArray result;
    QDataStream stream(&result, QIODeviceBase::WriteOnly);
    stream.setFloatingPointPrecision(QDataStream::SinglePrecision);
    stream.setVersion(QDataStream::Qt_6_3);

    stream << HorizontalVectorX;
    stream << HorizontalVectorY;
    stream << VericalThrust;
    stream << Depth;
    stream << AngularVelocityZ;
    stream << ManipulatorState;
    stream << ManipulatorRotate;
    stream << CameraRotate;
    stream << ResetInitialization;
    stream << LightsState;
    // TODO: Проверить размер структуры
    // Исходная структура C# имеет размер 36 байт ???
    // Добавляем два пустых байта в конец массива
    // stream << int8_t(0);
    // stream << int8_t(0);

    return result;
}

void SevROVControlData::printDebugInfo()
{
    std::string datetime = QDateTime::currentDateTime()
                               .toString("dd/MM/yyyy hh:mm:ss").toStdString();
    quint64 timestamp = QDateTime::currentMSecsSinceEpoch();
    qDebug() << "=============================================================";
    qDebug() << "CONTROL" << this->toByteArray().size() << "[bytes]"
             << datetime.c_str() << "[" << timestamp << "]";
    qDebug() << "=============================================================";
    qDebug() << "HorizontalVectorX:\t" << toFloatString(HorizontalVectorX).c_str();
    qDebug() << "HorizontalVectorY:\t" << toFloatString(HorizontalVectorY).c_str();
    qDebug() << "VericalThrust:\t\t" << toFloatString(VericalThrust).c_str();
    qDebug() << "Depth:\t\t\t" << toFloatString(Depth).c_str();
    qDebug() << "AngularVelocityZ:\t\t" << toFloatString(AngularVelocityZ).c_str();
    qDebug() << "ManipulatorState:\t\t" << toFloatString(ManipulatorState).c_str();
    qDebug() << "ManipulatorRotate:\t" << toFloatString(ManipulatorRotate).c_str();
    qDebug() << "CameraRotate:\t\t" << toFloatString(CameraRotate).c_str();
    qDebug() << "ResetInitialization:\t" << toIntString(ResetInitialization).c_str();
    qDebug() << "LightsState:\t\t" << toIntString(LightsState).c_str();
    qDebug() << "";
}
