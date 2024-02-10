#include "main.h"
#include "devices.hh"
#include "controls.h"

bool hang_active = false;

void hang_trigger() {
    while(true) {
        if (master.get_digital(master_UP)) {
            hang_active = !hang_active;
            while (master.get_digital(master_UP)) {
                pros::delay(10);
            }
        }
    }
    pros::delay(10);
}

void handle_hang() {
    // if (hang_active) {
    //     pros::lcd::set_text(1, "Should activate hang");
    //     hang.set_value(1);
    // }

    // else {
    //     pros::lcd::set_text(1, "Should de-activate hang");

    //     hang.set_value(0);
    // }
    while (master.get_digital(master_UP)) {
        master.set_text(0, 0, "Hang should be deploying");
        hang.set_value(1);
        pros::delay(100);
    }

    if (!(master.get_digital(master_UP))) {
        master.set_text(0, 0, "Hang should be closing");
        hang.set_value(0);
        pros::delay(100);
    }    
}