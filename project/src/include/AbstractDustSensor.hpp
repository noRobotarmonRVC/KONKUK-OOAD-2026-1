#pragma once

#include "DeviceComponent.hpp"

class AbstractDustSensor : public DeviceComponent {
   protected:
    bool is_dust_detected = false;

   public:
    bool isOn() const override;
    void turnOn() override;
    void turnOff() override;
    virtual bool findDust() = 0;
};
