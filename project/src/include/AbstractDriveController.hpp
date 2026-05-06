#pragma once

#include <array>

class AbstractDriveController {
public:
    virtual ~AbstractDriveController() = default;

    virtual void avoid(const std::array<int, 4>& obstacleInfo) = 0;
    virtual void moveForward() = 0;
    virtual void stop() = 0;
};