#ifndef SEVROVCONTROLDATA_H
#define SEVROVCONTROLDATA_H


#include "sevrovdata.h"

#include <cstdint>
#include <QByteArray>
#include <QDataStream>
#include <QDateTime>

class SevROVControlData : public SevROVData
{
public:
    SevROVControlData();

    void Initialize() override;
    void Initialize(float horizontalvectorx,
                    float horizontalvectory,
                    float verticalthrust,
                    float depth,
                    float angularVelocityz,
                    float manipulatorstate,
                    float manipulatorrotate,
                    float camerarotate,
                    int8_t resetinitialization,
                    int8_t lightstate);

    void setHorizontalVectorX(float value);
    void setHorizontalVectorY(float value);
    void setVericalThrust(float value);
    void setDepth(float value);
    void setAngularVelocityZ(float value);
    void setManipulatorState(float value);
    void setManipulatorRotate(float value);
    void setCameraRotate(float value);
    void setResetInitialization(int8_t value);
    void setLightsState(int8_t value);

    float getHorizontalVectorX();
    float getHorizontalVectorY();
    float getVericalThrust();
    float getDepth();
    float getAngularVelocityZ();
    float getManipulatorState();
    float getManipulatorRotate();
    float getCameraRotate();
    int8_t getResetInitialization();
    int8_t getLightsState();

    QByteArray toByteArray() override;
    void printDebugInfo() override;

private:
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
};

#endif // SEVROVCONTROLDATA_H
