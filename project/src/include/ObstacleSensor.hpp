#pragma once

#include "AbstractObstacleSensor.hpp"

#include <array>

class ObstacleSensor : public AbstractObstacleSensor {
public:
    ObstacleSensor() = default;
    explicit ObstacleSensor(const std::array<int, 4>& initialObstacleInfo);

    std::array<int, 4> findObstacle() override;
    void setObstacleInfo(const std::array<int, 4>& obstacleInfo);
};
