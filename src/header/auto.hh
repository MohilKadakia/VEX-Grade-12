#ifndef _auto_HH_
#define _auto_HH_
double get_avg_motor_position();
double get_distance_in_degrees(double distance_m);
void move(double meters, double kP, double kI, double kD); // , double kPturn, double kIturn, double kDturn
void move_forward(double distance_cm, double lowest_speed, double fastest_speed);
void move_radius(double radius, double width, double voltage, bool left);
void move_forward_inertial_pid(double distance_cm, double lowest_base_speed, double fastest_base_speed);
void turn(double degrees, double kP, double kI, double kD);
#endif