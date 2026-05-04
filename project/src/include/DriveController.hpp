#pragma once

#include <array>
#include <memory>

#include "AbstractDriveMotor.hpp"

class DriveController {
   private:
    std::shared_ptr<AbstractDriveMotor> motor;

   public:
    explicit DriveController(std::shared_ptr<AbstractDriveMotor> motor);

    void avoid(const std::array<int, 4>& obstacle_info);
    void moveForward();
    void stop();
};
