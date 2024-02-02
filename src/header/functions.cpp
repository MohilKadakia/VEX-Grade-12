#include <cmath>
#include "main.h"
#include "devices.hh"

double pid(double error, double* pe, double* in, double kp, double ki, double kd) {
    double derivative = error - *pe;
    if ((*pe > 0 && error < 0) || (*pe < 0 && error > 0))
      *in = 0; // remove integral at zero error
    double speed = error * kp + *in * ki + derivative * kd;

    // only let integral wind up if near the target
    if (fabs(error) < 15) {
    *in += error;
    }

    *pe = error;

    return speed;
}

void VeloNDispViaAccel(double* velox, double* veloy, double* veloz, double* dispx, double* dispy, double* dispz, double* pt ){
    double dt = (double) pros::millis()/1000 - *pt;
    *velox += IMU.get_accel().x*dt;
    *veloy += IMU.get_accel().y*dt;
    *veloz += IMU.get_accel().z*dt;
    *dispx += *velox*dt;
    *dispy += *veloy*dt;
    *dispz += *veloz*dt;
    *pt = (double) pros::millis()/1000;
		pros::lcd::set_text(0, std::to_string(dt));

}
void reset_inertial()
{
    IMU.reset(true);
}