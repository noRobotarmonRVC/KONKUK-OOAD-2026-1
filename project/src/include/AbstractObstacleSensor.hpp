#pragma once

#include <array>

#include "DeviceComponent.hpp"

class AbstractObstacleSensor : public DeviceComponent {
   protected:
    std::array<int, 4> obstacle_info = {0, 0, 0, 0};

   public:
    bool isOn() const override;
    void turnOn() override;
    void turnOff() override;
    virtual std::array<int, 4> findObstacle() = 0;
};
