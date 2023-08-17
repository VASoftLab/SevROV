#ifndef SEVROVLIBRARY_H
#define SEVROVLIBRARY_H

#include <cstdlib>
#include "sevrovcontroldata.h"

// https://support.xbox.com/en-US/help/hardware-network/controller/xbox-one-wireless-controller
struct XboxGamepad {
    short LStickX;  // 0
    short LStickY;  // 1
    short RStickX;  // 2
    short RStickY;  // 3
    short LTrigger; // 4
    short RTrigger; // 5
    short DPad;     // Directional Pad
    short A;        // 0
    short B;        // 1
    short X;        // 2
    short Y;        // 3
    short LBumper;  // 4
    short RBumper;  // 5
    short View;     // 6
    short Menu;     // 7
};

enum xbox_axis {
    LStickX,    // 0
    LStickY,    // 1
    RStickX,    // 2
    RStickY,    // 3
    LTrigger,   // 4
    RTrigger    // 5
};
enum xbox_butn {
    A,          // 0
    B,          // 1
    X,          // 2
    Y,          // 3
    LBumper,    // 4
    RBumper,    // 5
    View,       // 6
    Menu        // 7
};

const int JOYSTICK_DEAD_ZONE = 2500;

class SevROVLibrary
{
public:
    static void XboxToControlData(const XboxGamepad xbox, SevROVControlData *data);
};

#endif // SEVROVLIBRARY_H
