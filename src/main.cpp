#include "main.h"
#include "header/devices.h"
#include "header/pid.hh"
#include "header/ports.h"
#include <string>

bool catapult_active = false;
double pid_previous_time = 0;
double pid_previous_error = 0;
double pid_intergal = 0;
void reset_inertial()
{
	for (int i = 0; i < 2; i++){
		IMU[i].reset();
	}
}
void drive()
{
	int moveL = master.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y) + master.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_X);
	int moveR = master.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y) - master.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_X);
	left_motors.move(std::clamp(moveL, -127, 127));
	right_motors.move(std::clamp(moveR, -127, 127));
}


void initialize()
{
	pros::lcd::initialize();
	ResetInertialSensors();
}
void disabled() {
	pros::lcd::clear();
	while (1){
		pros::lcd::set_text(0, "Disabled");
		pros::delay(10);
	}
}
void competition_initialize()
{
	ResetInertialSensors();
}
void autonomous()
{
	pros::lcd::clear();
	ResetInertialSensors();
	while (1){
		pros::lcd::set_text(0, "Auto");
		pros::delay(10);
	}
}
void debug_values()
void catapult_trigger()
{	
	while(true) {
		if (master.get_digital(pros::E_CONTROLLER_DIGITAL_R2))
		{
			catapult_active = !catapult_active;
			pros::delay(90);
		}
		pros::delay(10);
	}
}
{
	pros::lcd::set_text(0, "Displacement: LMtop:" + std::to_string(left_motor_1.get_encoder_units()) + " LMfro" + std::to_string(left_motor_2.get_encoder_units()) + " LMbac" + std::to_string(left_motor_3.get_encoder_units()));
	pros::lcd::set_text(1, "Displacement: RMtop:" + std::to_string(right_motor_1.get_encoder_units()) + " RMfro" + std::to_string(right_motor_2.get_encoder_units()) + " RMbac" + std::to_string(right_motor_3.get_encoder_units()));
	pros::lcd::set_text(2, "RPM: LMtop:" + std::to_string(left_motor_1.get_actual_velocity()) + " LMfro" + std::to_string(left_motor_2.get_actual_velocity()) + " LMbac" + std::to_string(left_motor_3.get_actual_velocity()));
	pros::lcd::set_text(3, "RPM: RMtop:" + std::to_string(right_motor_1.get_actual_velocity()) + " RMfro" + std::to_string(right_motor_2.get_actual_velocity()) + " RMbac" + std::to_string(right_motor_3.get_actual_velocity()));
	pros::lcd::set_text(4, "Yaw: ISfro" + std::to_string(IMU[0].get_yaw()) + "ISbac" + std::to_string(IMU[1].get_yaw()));
	pros::lcd::set_text(5, "Catapult: " + std::to_string(catapult_active));
}
void opcontrol()
{
	pros::lcd::clear();
	ResetInertialSensors();

	pros::Task cata_trigger(catapult_trigger);
	while (true)
	{
		drive();
		debug_values(); 
		if (catapult_active) {
			catapult_motor.move_absolute(1000.0, 600);
        }
		pros::delay(10);
    }	
}