#include "DriveController.hpp"

#include <iostream>
#include <stdexcept>
#include <utility>

DriveController::DriveController(std::shared_ptr<AbstractDriveMotor> motor)
    : motor(std::move(motor)) {
    if (!motor) {
        throw std::invalid_argument("DriveController: motor is null");
    }
}

void DriveController::avoid(const std::array<int, 4>& obstacleInfo) {
    // index rule: [0] front, [1] right, [2] back, [3] left
    const bool frontBlocked = obstacleInfo[0] != 0;
    const bool rightBlocked = obstacleInfo[1] != 0;
    const bool backBlocked = obstacleInfo[2] != 0;
    const bool leftBlocked = obstacleInfo[3] != 0;

    if (!frontBlocked) {
        moveForward();
        return;
    }

    motor->stop();

    if (!rightBlocked) {
        motor->rotateRight();
    } else if (!leftBlocked) {
        motor->rotateLeft();
    } else if (!backBlocked) {
        motor->moveBackward();
    } else {
        std::cout << "[DriveController] all directions blocked\n";
    }
}

void DriveController::moveForward() {
    motor->moveForward();
}

void DriveController::stop() {
    motor->stop();
}
