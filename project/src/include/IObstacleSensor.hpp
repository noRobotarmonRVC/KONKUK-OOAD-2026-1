#pragma once

#include "DeviceComponent.hpp"

#include <array>

class IObstacleSensor : public DeviceComponent {
public:
    ~IObstacleSensor() override = default;

    virtual std::array<int, 4> findObstacle() = 0;
};
