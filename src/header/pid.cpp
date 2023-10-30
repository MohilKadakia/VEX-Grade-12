#include <cmath>
#include "main.h"
double pid(double error, double* pe, double* in, double kp, double ki, double kd)
{
    double derivative = error - *pe;
    //If it overshoots reset integral
    if ((*pe > 0 && error < 0) || (*pe < 0 && error > 0))
    {
        *in = 0;
    }
    double speed = error * kp + *in * ki + derivative * kd;

    //Only count integral if close
    if (fabs(error) < 15) {
    *in += error;
    }

    *pe = error;

    return speed;
}
double pidReal(double error, double* pe, double* pt, double* in, double kp, double ki, double kd)
{
    double current_time = (double) pros::micros()/1000000.0;
    double derivative = (error - *pe)/(current_time - *pt);
    //If it overshoots reset integral
    if ((*pe > 0 && error < 0) || (*pe < 0 && error > 0))
    {
        *in = 0;
    }
    *in += (*pe * (current_time - *pt)) + ((error - *pe)*(current_time - *pt)*(0.5));
    double speed = error * kp + *in * ki + derivative * kd;
    *pe = error;
    *pt = current_time;
    return speed;
}