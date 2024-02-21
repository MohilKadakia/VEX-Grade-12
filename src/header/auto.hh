#ifndef _auto_HH_
#define _auto_HH_
void move(double meters, double kP, double kI, double kD, double kPturn, double kIturn, double kDturn);
void move_radius(double radius, double width, double voltage, bool left);
void turn(double degrees, double kP, double kI, double kD);
#endif