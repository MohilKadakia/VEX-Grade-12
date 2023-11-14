#include "devices.hh"
#include "functions.hh"

double integral_turn = 0;
double previous_error_turn = 0;

double change_numbers(double value){
    if (value < 0){
        return value+360;
    }
    return value;
}

void turn_left(double degree) {
    double avgInertialYaw = (IMU[0].get_yaw()+IMU[1].get_yaw())/2;
    bool buffer_period_active = false;
    while (fabs(degree-change_numbers(avgInertialYaw))>1){
        avgInertialYaw = (IMU[0].get_yaw()+IMU[1].get_yaw())/2;
        double output = pid(degree-change_numbers(avgInertialYaw), &previous_error_turn, &integral_turn, 2.0, 0.1, 0.05);
        // master.set_text(0,0,std::to_string(buffer_period_active));
        // master.set_text(1,0,std::to_string(output));
        // pros::lcd::set_text(0,std::to_string((int) std::round(avgInertialYaw)) + " " + std::to_string((int) round(change_numbers(avgInertialYaw)))+ " " + std::to_string(buffer_period_active));
        left_motors.move_velocity(output);
        right_motors.move_velocity(-output);
        pros::delay(10);
    }
    master.clear();
    // int bufferTime = pros::millis()+3000;
    // while(pros::millis() < bufferTime) {
    //     buffer_period_active = true;
    //     avgInertialYaw = (IMU[0].get_yaw()+IMU[1].get_yaw())/2;
    //     master.set_text(0,0,std::to_string((int) std::round(avgInertialYaw)) + " " + std::to_string((int) round(change_numbers(avgInertialYaw)))+ " " + std::to_string(buffer_period_active));
    //     pros::lcd::set_text(0,std::to_string((int) std::round(avgInertialYaw)) + " " + std::to_string((int) round(change_numbers(avgInertialYaw)))+ " " + std::to_string(buffer_period_active));
    //     double output = pid(degree-change_numbers(avgInertialYaw), &previous_error_turn, &integral_turn, 0.50, 0.2, 0.05);
    //     left_motors.move_velocity(output);
    //     right_motors.move_velocity(-output);
    //     pros::delay(20);
    // }
    buffer_period_active = false;
}

void turn_right(double degree) {
    double avgInertialYaw = 0;
    double startAngle = (IMU[0].get_yaw()+IMU[1].get_yaw())/2;
    while (fabs(degree-avgInertialYaw)>0.1){
        avgInertialYaw = (IMU[0].get_yaw()+IMU[1].get_yaw())/2;
        double output = pid(fabs(degree-(avgInertialYaw-startAngle)), &previous_error_turn, &integral_turn, 1.66, 0.15, 0.75);
        left_motors.move_velocity(output);
        right_motors.move_velocity(-1.0*output);
        pros::delay(10);
    }
}

void turn(double targetAngle) {
    double avgInertialYaw = 0;
    while (fabs(targetAngle-avgInertialYaw)>0.1){
        avgInertialYaw = (IMU[0].get_yaw()+IMU[1].get_yaw())/2;
        double output = pid(targetAngle-avgInertialYaw, &previous_error_turn, &integral_turn, 1.66, 0.15, 0.75);
        left_motors.move_velocity(output);
        right_motors.move_velocity(-1.0*output);
        pros::delay(10);
    }
}