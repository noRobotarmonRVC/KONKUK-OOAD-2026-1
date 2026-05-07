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

void DriveController::avoid(const std::array<int, 4>& obstacleInfo) {
    // index rule: [0] front, [1] left, [2] right, [3] back
    const bool frontBlocked = obstacleInfo[0] != 0;
    const bool leftBlocked = obstacleInfo[1] != 0;
    const bool rightBlocked = obstacleInfo[2] != 0;
    const bool backBlocked = obstacleInfo[3] != 0;

    if (!frontBlocked) {
        throw std::invalid_argument("Drivecontroller: front obstacle not handled before");
    }

    if (!leftBlocked) {
        motor->rotateLeft();
    } else if (!rightBlocked) {
        motor->rotateRight();
    } else if (rightBlocked && leftBlocked && !backBlocked) {
        motor->moveBackward();
        // RVC가 뒤로 갈 시간을 기다리기
        sleep(2);
        motor->stop();
        motor->rotateLeft();
    } else {
        throw std::invalid_argument("DriveController: obstacle in all sides exception not handled");
    }
}

void DriveController::moveForward() {
    motor->moveForward();
}

void DriveController::stop() {
    motor->stop();
}
