#pragma once

#include "IObstacleSensor.hpp"

#include <array>

class AbstractObstacleSensor : public IObstacleSensor {
protected:
    std::array<int, 4> obstacleInfo_ = {0, 0, 0, 0};

public:
    bool isOn() const override;
    void turnOn() override;
    void turnOff() override;
};
