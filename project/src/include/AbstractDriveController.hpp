#pragma once

#include <array>

class AbstractDriveController {
public:
    virtual ~AbstractDriveController() = default;

    virtual bool avoid(const std::array<int, 2>& obstacleInfo,int flag) = 0;
    virtual void moveForward() = 0;
    virtual void stop() = 0;
};