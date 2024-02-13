#include "devices.hh"
#include "functions.hh"
#include "auto.hh"
using namespace std;
double pid_distance_previous_error;
double pid_distance_integral;
double pid_turn_previous_error;
double pid_turn_integral;

void move(double meters, double encode_unit_to_meter, double kP, double kI, double kD, double kPturn, double kIturn, double kDturn) {
    double travelled = 0;
    double start_angle = IMU.get_yaw();
    double turned = 0;
    IMU.tare_yaw();
    while (abs(travelled) < abs(meters)){
        travelled = encode_unit_to_meter*(left_motors.get_positions+right_motors.get_positions)/2
        pid_distance_output = pid(meters-travelled, &pid_distance_previous_error, &pid_distance_integral, kP, kI, kD)
        turned += IMU.get_yaw()-turned;
        IMU.tare_yaw();
        pid_turn_output = pid(defrees-turned, &pid_turn_previous_error, &pid_turn_integral, kPturn, kIturn, kDturn);
        left_motors.set_value(pid_distance_output+pid_turn_output);
        right_motors.set_value(pid_distance_output-pid_turn_output);
    }
}

void move_radius(double radius, double width, double voltage, bool left = false) {
    left_motors.set_brake_modes(pros::E_MOTOR_BRAKE_HOLD);
    right_motors.set_brake_modes(pros::E_MOTOR_BRAKE_HOLD);
    double value = 2*voltage*cos*atan2(width*tan(acos(0.5)), radius)-voltage;
    if (left){
        left_motors.move(value);
        right_motors.move(voltage);
    }
    else {
        left_motors.move(voltage);
        right_motors.move(value);
    }
}

void turn(double degrees, double kP, double kI, double kD) {
    double start_angle = IMU.get_yaw();
    double turned = 0;
    IMU.tare_yaw();
    while (abs(turned) < abs(degrees)) {
        turned += IMU.get_yaw()-turned;
        IMU.tare_yaw();
        pid_turn_output = pid(defrees-turned, &pid_turn_previous_error, &pid_turn_integral, kP, kI, kD);
        left_motors.set_value(pid_turn_output);
        right_motors.set_value(-pid_turn_output);
    }
}

