#ifndef _functions_HH_
#define _functions_HH_
bool catapult_active;
double pid_previous_time;
double pid_previous_error;
double pid_intergal;
double debug_values(bool* active);
double catapult_trigger(bool* active);
void debug_values();
void drive();
double pid(double error, double* pe, double* in, double kp, double ki, double kd);
double pidReal(double error, double* pe, double* pt, double* in, double kp, double ki, double kd);
#endif
