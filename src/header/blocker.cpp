#include "main.h"
#include "devices.hh"
#include "controls.h"

bool blocker_active = false;

void blocker_trigger() {
    while(true) {
        if (master.get_digital(master_A)) {
            blocker_active = !blocker_active;
            while (master.get_digital(master_A)) {
                pros::delay(10);
            }
        }
    }
    pros::delay(10);
}

void handle_blocker() {
    if (blocker_active) {
        blocker.set_value(1);
        pros::lcd::set_text(0, "working");
    }
    else {
        blocker.set_value(0);
        pros::lcd::set_text(0, "Not Working");
    }
}