#include "devices.hh"
#include "functions.hh"
#include "auto.hh"

void move(double meters) {
    double travelled;
    while (travelled < meters){

    }
}

void move_radius() {

}

void turn(double degrees) {
    double start_angle = IMU.get_yaw();
    double turned = 0;
    IMU.tare_yaw();
    while (turned < degrees) {
        turned += IMU.get_yaw()-turned;
        IMU.tare_yaw();
    }
}
using namespace std;

