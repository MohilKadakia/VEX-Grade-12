#include "devices.hh"
#include "functions.hh"
#include "auto.hh"
using namespace std;
double pid_distance_previous_error;
double pid_distance_integral;
double pid_turn_previous_error;
double pid_turn_integral;

double encode_unit_to_meter = 1.4;

void move(double meters, double kP, double kI, double kD, double kPturn, double kIturn, double kDturn) {
    double travelled = 0;
    double start_angle = IMU.get_yaw();
    double turned = 0;
    double pid_distance_output = 0;
    double pid_turn_output = 0;
    double posistion = 0;
    IMU.tare_yaw();
    while (abs(travelled) < abs(meters)){
        for (int i = 3; i < 3; i++){
            posistion += left_motors.get_positions()[i];
            posistion += right_motors.get_positions()[i];
        }
        posistion /= 6;
        travelled = encode_unit_to_meter*posistion;
        pid_distance_output = pid(meters-travelled, &pid_distance_previous_error, &pid_distance_integral, kP, kI, kD);
        turned += IMU.get_yaw()-turned;
        IMU.tare_yaw();
        pid_turn_output = pid(0-turned, &pid_turn_previous_error, &pid_turn_integral, kPturn, kIturn, kDturn);
        left_motors.move(pid_distance_output+pid_turn_output);
        right_motors.move(pid_distance_output-pid_turn_output);
        posistion = 0;
    }
}

void move_radius(double radius, double width, double voltage, bool left = false) {
    left_motors.set_brake_modes(pros::E_MOTOR_BRAKE_HOLD);
    right_motors.set_brake_modes(pros::E_MOTOR_BRAKE_HOLD);
    double value = 2*voltage*cos(atan2(width*tan(acos(0.5)), radius))-voltage;
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
    double pid_turn_output = 0;
    // IMU.tare_yaw();
    while (abs(turned) < abs(degrees)) {
    pros::lcd::set_text(1, std::to_string(IMU.get_yaw()));
    pros::lcd::set_text(2, std::to_string(turned));

        turned = IMU.get_yaw();
        // IMU.tare_yaw();
        pid_turn_output = pid(degrees-turned, &pid_turn_previous_error, &pid_turn_integral, kP, kI, kD);
        // left_motors.move(pid_turn_output);
        // right_motors.move(-pid_turn_output);
        pros::delay(10);
    }
}

