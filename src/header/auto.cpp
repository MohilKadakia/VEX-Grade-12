#include "devices.hh"
#include "functions.hh"
#include "auto.hh"
using namespace std;
double pid_distance_previous_error;
double pid_distance_integral;
double pid_turn_previous_error;
double pid_turn_integral;

double encode_unit_to_meter = 1.4;

double get_distance_in_degrees(double distance_cm) {
    double wheel_circumfrence = 2 * 3.14159265 * 4; 
    double distance_degrees = (360 * distance_cm) / wheel_circumfrence;
    return distance_degrees*1.66;
}

double get_avg_motor_position() {
    // Returns average motor position of robot in degrees
    
    double avg_motor_rotation_left = 0;
    double avg_motor_rotation_right = 0;

    for(int i = 0; i < 3; i++) {
        avg_motor_rotation_left += left_motors.get_positions()[i];
        avg_motor_rotation_right += right_motors.get_positions()[i];
    }

    avg_motor_rotation_left /= 3;
    avg_motor_rotation_right /= 3;
    // master.set_text(0, 0, std::to_string(avg_motor_rotation_left));
    // master.set_text(1, 0, std::to_string(avg_motor_rotation_right));    
    double avg_rotation = (avg_motor_rotation_left + avg_motor_rotation_right) / 2;
    // master.set_text(0, 0, std::to_string(avg_rotation));
    return avg_rotation;
}

void move(double meters, double kP, double kI, double kD) {
    double travelled = 0;
    double start_angle = IMU.get_yaw();
    double turned = 0;
    double pid_distance_output = 0;
    double pid_turn_output = 0;
    double start_position = get_avg_motor_position();
    double posistion = start_position;

    while (abs(travelled) < abs(meters)){
        // master.set_text(0, 0, std::to_string(IMU.get_yaw()));
        // master.set_text(2, 0, std::to_string(pid_distance_output));
        posistion = get_avg_motor_position();
        travelled = (encode_unit_to_meter*posistion);
        // master.set_text(0, 0, std::to_string(posistion));
        pid_distance_output = pid(meters-travelled, &pid_distance_previous_error, &pid_distance_integral, kP, kI, kD);
        // IMU.tare_yaw();
        right_motors.move(15);
        left_motors.move(15*0.9);
        // pid_turn_output = pid(start_angle - turned, &pid_turn_previous_error, &pid_turn_integral, kPturn, kIturn, kDturn);
        // left_motors.move(pid_distance_output+pid_turn_output);
        // right_motors.move(pid_distance_output-pid_turn_output);
        posistion = 0;
    }
    right_motors.move(0);
    left_motors.move(0);
    master.set_text(0, 0, "Finished forward");
}

// THIS WORKS
void move_forward(double distance_cm, double lowest_speed, double fastest_speed) {
    // converts cm to degrees to be compared with motor positions (in degrees) 
    double distance_degrees = get_distance_in_degrees(distance_cm);

    // the start and current position is the same
    double avg_start_rotation = get_avg_motor_position();
    double avg_current_rotation = avg_start_rotation;

    double start_angle = IMU.get_yaw();
    // double pid_previous_error_turn = 0;
    // double pid_integral_turn = 0;

    // pid move forward
    while(avg_current_rotation <= avg_start_rotation + distance_degrees) {
        avg_current_rotation = get_avg_motor_position();

        double error = (avg_start_rotation + distance_degrees) - avg_current_rotation;
        double output = pid(error, &pid_distance_previous_error, &pid_distance_integral, 2.0, 0.1, 0.05);
        // master.set_text(0, 0, std::to_string(error));
        left_motors.move(std::clamp(output, lowest_speed, fastest_speed)*0.9);
        right_motors.move(std::clamp(output, lowest_speed, fastest_speed));
        pros::delay(200);
    }
    left_motors.move(0);
    right_motors.move(0);
}

void move_forward_inertial_pid(double distance_cm, double lowest_base_speed, double fastest_base_speed) {
    // converts cm to degrees to be compared with motor positions (in degrees) 
    double distance_degrees = get_distance_in_degrees(distance_cm);

    // calculates base angle
    double average_inertial_start_angle = IMU.get_yaw();

    // the start and current position is the same
    double avg_start_rotation = get_avg_motor_position();
    double avg_current_rotation = avg_start_rotation;

    while(avg_current_rotation <= avg_start_rotation + distance_degrees) {
        avg_current_rotation =  get_avg_motor_position();

        double current_inertial_angle = IMU.get_yaw();
        double inertial_error = average_inertial_start_angle - current_inertial_angle;
        double inertial_speed_output = pid(inertial_error, &pid_turn_previous_error, &pid_turn_integral, 1.0, 0.1, 0.05);

        double distance_error = (avg_start_rotation + distance_degrees) - avg_current_rotation;
        double distance_speed_output = pid(distance_error, &pid_distance_previous_error, &pid_distance_integral, 0.05, 0.0001, 0.001);

        left_motors.move((std::clamp(distance_speed_output, lowest_base_speed, fastest_base_speed) + inertial_speed_output)*0.95);
        right_motors.move(std::clamp(distance_speed_output, lowest_base_speed, fastest_base_speed) - inertial_speed_output);
        pros::delay(100);
        if (abs(distance_speed_output) < 10) {
            break;
        }        
    }
    left_motors.move(0);
    right_motors.move(0);
    left_motors.brake();
    right_motors.brake();
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
    double turned = 0;
    double pid_turn_output = 0;
    pros::delay(100);

    while (abs(turned) < abs(degrees)-3) {
    // while (true) {
    pros::lcd::set_text(1, std::to_string(IMU.get_yaw()));
    pros::lcd::set_text(2, std::to_string(turned));

        turned = IMU.get_yaw();
        master.clear();
        // IMU.tare_yaw();
        master.set_text(0,0, std::to_string(IMU.get_yaw()));
        master.set_text(1,0, std::to_string(IMU.get_yaw()));
        master.set_text(2,0, std::to_string(IMU.get_yaw()));

        pid_turn_output = pid(degrees-turned, &pid_turn_previous_error, &pid_turn_integral, kP, kI, kD);
        left_motors.move(pid_turn_output);
        right_motors.move(-pid_turn_output);
        pros::delay(20);
        
    }
    left_motors.move(0);
    right_motors.move(0);

    IMU.tare_yaw();
    pros::delay(100);

}
