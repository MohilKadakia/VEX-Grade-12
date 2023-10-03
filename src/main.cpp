#include "main.h"
#include <string>
#include "ARMS/api.h"
#include "ARMS/config.h"

void initialize() {
	arms::init();
	pros::lcd::initialize();
}

void disabled() {}

void competition_initialize() {}

void autonomous() {}

void opcontrol() {
	while (true) {
		
    }	
}
