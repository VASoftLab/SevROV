#include "sevrovlibrary.h"

void SevROVLibrary::XboxToControlData(const XboxGamepad xbox, SevROVControlData *data)
{
    auto leftX  = xbox.LStickX / SHRT_MAX;
    auto leftY  = xbox.LStickY / SHRT_MAX;
    data->setHorizontalVectorX(leftX);
    data->setHorizontalVectorY(leftY);

    auto rightX  = xbox.RStickX / SHRT_MAX;
    auto rightY  = xbox.RStickY / SHRT_MAX;
    data->setAngularVelocityZ(rightX);
    data->setVericalThrust(rightY);

    auto depth = 0;
    data->setDepth(depth);

    auto gripDirection = xbox.LBumper - xbox.RBumper;
    data->setManipulatorState(gripDirection);

    auto manipulatorRotateDirection = (xbox.LTrigger / SHRT_MAX - xbox.RTrigger/ SHRT_MAX) / 2;
    data->setManipulatorRotate(manipulatorRotateDirection);

    auto cameraRotateDirection = ((xbox.DPad == 1) ? 1 : 0) - ((xbox.DPad == 4) ? 1 : 0);
    data->setCameraRotate(cameraRotateDirection);

    auto reset = xbox.Menu;
    data->setResetInitialization(reset);

    if ((uint8_t)xbox.Y == 1) // Меняем состояние при нажатой кнопке Y
    {
        switch ((uint8_t)data->getLightsState())
        {
        case 0:
            data->setLightsState(1);
            break;
        case 1:
            data->setLightsState(0);
            break;
        }
    }
}
