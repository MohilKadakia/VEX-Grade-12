#include "main.h"
#include "devices.hh"
#include "controls.h"

bool intake_active = false;

void intake_trigger() {
    while(true) {
        if (master.get_digital(master_L1)) {
            intake_active = !intake_active;
            while (master.get_digital(master_L1)) {
                pros::delay(10);
            }
        }
    }
    pros::delay(10);
}

void handle_intake() {
    if (intake_active) {
        intake.set_value(1);
        pros::lcd::set_text(0, "working");
    }
    else {
        intake.set_value(0);
        pros::lcd::set_text(0, "Not Working");
    }
}