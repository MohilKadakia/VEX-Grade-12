#ifndef _auto_HH_
#define _auto_HH_
extern double integral_turn;
extern double previous_error_turn;
double change_numbers(double value);
void turn(double degree);
void move(double cm);
void turn_right_to_look_at(double degrees);
void turn_left_to_look_at(double degrees);
void move_backward(double cm);
#endif