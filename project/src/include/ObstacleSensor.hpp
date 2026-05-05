#pragma once

#include <array>

#include "AbstractObstacleSensor.hpp"

class ObstacleSensor : public AbstractObstacleSensor {
   public:
    ObstacleSensor() = default;
    explicit ObstacleSensor(const std::array<int, 4>& obstacle_info);

    bool isOn() const override;
    void turnOn() override;
    void turnOff() override;
    std::array<int, 4> findObstacle() override;
};
