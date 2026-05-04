#pragma once

#include "DeviceComponent.hpp"

class AbstractDustSensor : public DeviceComponent {
   protected:
    bool is_dust_detected = false;

   public:
    virtual bool isOn() const override = 0;
    virtual void turnOn() override = 0;
    virtual void turnOff() override = 0;
    virtual bool findDust() = 0;
};
