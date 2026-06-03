#pragma once

#include <array>
#include <memory>

#include "AbstractDriveController.hpp"
#include "AbstractDriveMotor.hpp"

class DriveController : public AbstractDriveController {
private:
    std::shared_ptr<AbstractDriveMotor> motor;

public:
    explicit DriveController(std::shared_ptr<AbstractDriveMotor> motor);

    bool avoid(const std::array<int, 2>& obstacleInfo, int flag) override;
    void moveForward() override;
    void stop() override;
};