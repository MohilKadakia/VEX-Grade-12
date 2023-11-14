#include "devices.hh"
#include "functions.hh"

double integral_turn = 0;
double previous_error_turn = 0;

double change_numbers(double value){
    if (std::round(value) < 0){
        return value+360;
    }
    return value;
}

void turn(double degree) {
    double avgInertialYaw = 0;
    bool buffer_period_active = false;
    master.clear();
    while (fabs(degree-avgInertialYaw)>1){
        avgInertialYaw = (change_numbers(IMU[0].get_yaw())+change_numbers(IMU[1].get_yaw()))/2;
        master.set_text(0,0,std::to_string(buffer_period_active) + " " + std::to_string(IMU[0].get_yaw()) + " " + std::to_string(IMU[1].get_yaw()));
        pros::lcd::set_text(0, std::to_string((int) round(avgInertialYaw))+ " " + std::to_string(buffer_period_active));
        double output = pid(degree-avgInertialYaw, &previous_error_turn, &integral_turn, 2.0, 0.1, 0.05);
        left_motors.move_velocity(output);
        right_motors.move_velocity(-output);
        pros::delay(20);
    }
    int bufferTime = pros::millis()+3000;
    while(pros::millis() < bufferTime) {
        buffer_period_active = true;
        avgInertialYaw = (change_numbers(IMU[0].get_yaw())+change_numbers(IMU[1].get_yaw()))/2;
        master.set_text(0,0,std::to_string(buffer_period_active) + " " + std::to_string(IMU[0].get_yaw()) + " " + std::to_string(IMU[1].get_yaw()));
        pros::lcd::set_text(0, std::to_string((int) round(avgInertialYaw))+ " " + std::to_string(buffer_period_active));
        double output = pid(degree-avgInertialYaw, &previous_error_turn, &integral_turn, 2.0, 0.1, 0.05);
        left_motors.move_velocity(output);
        right_motors.move_velocity(-output);
        pros::delay(20);
    }
    left_motors.move_velocity(0);
	right_motors.move_velocity(0);
    buffer_period_active = false;
}