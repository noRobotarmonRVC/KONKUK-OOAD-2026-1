#include "DriveController.hpp"

#include <unistd.h>

#include <iostream>
#include <stdexcept>
#include <utility>

DriveController::DriveController(std::shared_ptr<AbstractDriveMotor> motor)
    : motor(std::move(motor)) {
    if (!this->motor) {
        throw std::invalid_argument("DriveController: motor is null");
    }
}

int DriveController::avoid(const std::array<int, 2>& obstacleInfo,int flag) {
    // index rule: [0] front, [1] left, [2] right, [3] back
    const bool frontBlocked = obstacleInfo[0] != 0;
    const bool leftBlocked = obstacleInfo[1] != 0;

    if(flag ==0){
        if (leftBlocked==0) {
            motor->rotateLeft();
            return 0;
        }
        else{
            motor->rotateRight();
            return 1;
        }
    }
    else{
        if(frontBlocked==1){
            motor -> rotateLeft();
            motor -> moveForward();
            motor -> rotateLeft();
            return 0;
        }
    }
}

void DriveController::moveForward() {
    motor->moveForward();
}

void DriveController::stop() {
    motor->stop();
}
