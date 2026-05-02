#include "DriveController.hpp"

#include <iostream>
#include <stdexcept>
#include <utility>

DriveController::DriveController(std::shared_ptr<IDriveMotor> motor)
    : motor_(std::move(motor)) {
    if (!motor_) {
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

    motor_->stop();

    if (!rightBlocked) {
        motor_->rotateRight();
    } else if (!leftBlocked) {
        motor_->rotateLeft();
    } else if (!backBlocked) {
        motor_->moveBackward();
    } else {
        std::cout << "[DriveController] all directions blocked\n";
    }
}

void DriveController::moveForward() {
    motor_->moveForward();
}

void DriveController::stop() {
    motor_->stop();
}
