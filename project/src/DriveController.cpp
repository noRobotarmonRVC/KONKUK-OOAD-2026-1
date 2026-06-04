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

bool DriveController::avoid(const std::array<int, 2>& obstacleInfo, int flag) {
    // index rule: [0] front, [1] left
    const bool frontBlocked = obstacleInfo[0] != 0;
    const bool leftBlocked = obstacleInfo[1] != 0;

    std::cout << "[DriveController] flag=" << flag
              << ", frontBlocked=" << frontBlocked
              << ", leftBlocked=" << leftBlocked
              << std::endl;

    // 첫 번째 회피 판단
    if (flag == 0) {
        if (!leftBlocked) {
            std::cout << "Front is blocked, left is open. Rotating Left." << std::endl;
            motor->rotateLeft();
            return false;
        }

        std::cout << "Front and left are blocked. Rotating Right." << std::endl;
        motor->rotateRight();

        // 오른쪽으로 돈 뒤 다시 obstacle 확인하라고 알림
        return true;
    }

    // flag == 1
    // 오른쪽으로 돈 뒤 다시 확인했는데, 그 방향의 앞도 막혀 있으면
    // sequence diagram대로 rotateLeft -> moveBackward -> rotateLeft
    if (frontBlocked) {
        std::cout << "After rotating right, front is still blocked. "
                  << "Rotating Left, Moving Backward, Rotating Left."
                  << std::endl;

        motor->rotateLeft();
        motor->moveBackward();
        motor->rotateLeft();
    }

    return false;
}
void DriveController::moveForward() {
    motor->moveForward();
}

void DriveController::stop() {
    motor->stop();
}
