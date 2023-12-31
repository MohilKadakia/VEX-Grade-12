#include "main.h"
#include "devices.hh"
#include "controls.h"

bool intake_active_in = false;
bool intake_active_out = false;

void intake_trigger() {
    while(true) {
        if (master.get_digital(master_L1)) {
            intake_active_in = !intake_active_in;
            while (master.get_digital(master_L1)) {
                pros::delay(10);
            }
        }

        if (master.get_digital(master_R1)) {
            intake_active_out = !intake_active_out;
            while (master.get_digital(master_R1)) {
                pros::delay(10);
            }
        }
    }
    pros::delay(10);
}

void handle_intake() {
    if (intake_active_in) {
        intake_motors.move(100);
        pros::lcd::set_text(0, "moving forward");
    }
    
    else if (intake_active_out) {
        intake_motors.move(-100);
        pros::lcd::set_text(0, "moving backwards");
    }

    else {
        intake_motors.move(0);
        pros::lcd::set_text(0, "Not in action");
    }
}