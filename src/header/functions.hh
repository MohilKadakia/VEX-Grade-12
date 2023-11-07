#ifndef _functions_HH_
#define _functions_HH_
extern bool catapult_active;
extern bool wings_active;
extern double pid_previous_time;
extern double pid_previous_error;
extern double pid_intergal;
double debug_values();
double catapult_trigger();
void wings_trigger();
void reset_inertial();
void drive();
double pid(double error, double* pe, double* in, double kp, double ki, double kd);
double pidReal(double error, double* pe, double* pt, double* in, double kp, double ki, double kd);
#endif