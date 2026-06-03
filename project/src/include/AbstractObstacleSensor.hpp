#pragma once

#include <array>

#include "DeviceComponent.hpp"

class AbstractObstacleSensor : public DeviceComponent {
   protected:
    std::array<int, 2> obstacle_info = {0, 0};

   public:
    virtual bool isOn() const override = 0;
    virtual void turnOn() override = 0;
    virtual void turnOff() override = 0;
    virtual std::array<int, 2> findObstacle() = 0;
};
