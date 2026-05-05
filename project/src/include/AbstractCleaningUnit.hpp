#pragma once

#include "DeviceComponent.hpp"

class AbstractCleaningUnit : public DeviceComponent {
   protected:
    int power = 0;

   public:
    virtual bool isOn() const override = 0;
    virtual void turnOn() override = 0;
    virtual void turnOff() override = 0;
    virtual void clean(bool is_dust_detected) = 0;
    virtual void boostMode() = 0;
    virtual void normalMode() = 0;
};
