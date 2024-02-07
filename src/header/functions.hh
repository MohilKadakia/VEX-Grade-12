#ifndef _functions_HH_
#define _functions_HH_
double pid(double error, double* pe, double* in, double kp, double ki, double kd);
void VeloNDispViaAccel(double* velox, double* veloy, double* veloz, double* dispx, double* dispy, double* dispz, double* pt );
void reset_inertial();
#endif