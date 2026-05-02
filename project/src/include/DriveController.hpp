#pragma once

#include "IDriveMotor.hpp"

#include <array>
#include <memory>

class DriveController {
private:
    std::shared_ptr<IDriveMotor> motor_;

public:
    explicit DriveController(std::shared_ptr<IDriveMotor> motor);

    void avoid(const std::array<int, 4>& obstacleInfo);
    void moveForward();
    void stop();
};
