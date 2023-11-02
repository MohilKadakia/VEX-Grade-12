#include "main.h"
#include "devices.h"

bool wings_active = false;

void wings_pneumatic() {
    while(true) {
        if (wings_active) {
            pros::lcd::set_text(2, "Pneumatics Active");
            while (wings_active) {
                wings.set_value(true);
            }       
        } else {
            wings.set_value(false);
        }
    }
    pros::delay(10);
}