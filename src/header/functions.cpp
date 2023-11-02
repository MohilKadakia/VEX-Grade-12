#include "devices.h"

void reset_inertial()
{
    IMU[0].reset();
    IMU[1].reset();
}