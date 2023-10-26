#include "main.h"
#include "header/devices.h"
#include "header/pid.hh"
#include "header/ports.h"
#include <string>

bool cata_active = false;

void ResetInertialSensors()
{
	for (int i = 0; i < 2; i++){
		IMU[i].reset();
	}
}

void Drive(pros::Controller master)
{
	int moveL = master.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y) + master.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_X);
	int moveR = master.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y) - master.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_X);
	left_motors.move(std::clamp(moveL, -127, 127));
	right_motors.move(std::clamp(moveR, -127, 127));
}

void CatapultTrigger(pros::Controller master, bool* active)
{	
	bool cata_button = master.get_digital(pros::E_CONTROLLER_DIGITAL_R2);
	if (cata_button)
	{
		*active = !*active;
		
	}
	pros::delay(20);
}

void initialize()
{
	pros::lcd::initialize();
	ResetInertialSensors();
}

void disabled() {}

void competition_initialize()
{
	ResetInertialSensors();
}

void autonomous()
{
	ResetInertialSensors();
}

void DebugValues(pros::Controller master)
{
	pros::lcd::set_text(0, "Displacement: LMtop:" + std::to_string(left_motor_1.get_encoder_units()) + " LMfro" + std::to_string(left_motor_2.get_encoder_units()) + " LMbac" + std::to_string(left_motor_3.get_encoder_units()));
	pros::lcd::set_text(1, "Displacement: RMtop:" + std::to_string(right_motor_1.get_encoder_units()) + " RMfro" + std::to_string(right_motor_2.get_encoder_units()) + " RMbac" + std::to_string(right_motor_3.get_encoder_units()));
	pros::lcd::set_text(2, "RPM: LMtop:" + std::to_string(left_motor_1.get_actual_velocity()) + " LMfro" + std::to_string(left_motor_2.get_actual_velocity()) + " LMbac" + std::to_string(left_motor_3.get_actual_velocity()));
	pros::lcd::set_text(3, "RPM: RMtop:" + std::to_string(right_motor_1.get_actual_velocity()) + " RMfro" + std::to_string(right_motor_2.get_actual_velocity()) + " RMbac" + std::to_string(right_motor_3.get_actual_velocity()));
	pros::lcd::set_text(4, "Yaw: ISfro" + std::to_string(IMU[0].get_yaw()) + "ISbac" + std::to_string(IMU[1].get_yaw()));
	pros::lcd::set_text(5, "Catapult: " + std::to_string(cata_active));
}
void opcontrol()
{
	ResetInertialSensors();
	pros::Controller master(pros::E_CONTROLLER_MASTER);
	while (true)
	{
		Drive(master);
		DebugValues(master); 
		//pros::Task myTask(CatapultTrigger, (pros::Controller)"master");
		pros::delay(10);
    }	
}