#ifndef _functions_HH_
#define _functions_HH_
double pid(double error, double* pe, double* in, double kp, double ki, double kd);
void reset_inertial();
#endif