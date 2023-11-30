#ifndef _auto_HH_
#define _auto_HH_
extern double integral_turn;
extern double previous_error_turn;
double change_numbers(double value);
void turn(double degree);
void move(double cm);
void turn_right_to_look_at(double degrees);
void turn_left_to_look_at(double degrees);
void move_backward(double distance_cm, double lowest_speed, double fastest_speed);
void move_forward(double distance_cm, double lowest_speed, double fastest_speed);
void move_forward_inertial(double distance_cm, double lowest_base_speed, double fastest_base_speed, double base_speed);
#endif