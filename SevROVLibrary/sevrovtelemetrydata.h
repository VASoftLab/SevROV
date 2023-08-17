#ifndef SEVROVTELEMETRYDATA_H
#define SEVROVTELEMETRYDATA_H

#include "sevrovdata.h"

#include <QByteArray>
#include <QDataStream>
#include <QDateTime>

class SevROVTelemetryData : public SevROVData
{
public:
    SevROVTelemetryData();

    void Initialize() override;
    void Initialize(float roll,
                    float pitch,
                    float yaw,
                    float heading,
                    float depth);

    void setRoll(float value);
    void setPitch(float value);
    void setYaw(float value);
    void setHeading(float value);
    void setDepth(float value);

    float getRoll();
    float getPitch();
    float getYaw();
    float getHeading();
    float getDepth();

    QByteArray toByteArray() override;
    void printDebugInfo() override;

private:
    float Roll;
    float Pitch;
    float Yaw;
    float Heading;
    float Depth;
};

#endif // SEVROVTELEMETRYDATA_H
