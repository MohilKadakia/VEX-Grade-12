#ifndef _pid_HH_
#define _pid_HH_
double pid(double error, double* pe, double* in, double kp, double ki, double kd);
double pidReal(double error, double* pe, double* pt, double* in, double kp, double ki, double kd);
#endif