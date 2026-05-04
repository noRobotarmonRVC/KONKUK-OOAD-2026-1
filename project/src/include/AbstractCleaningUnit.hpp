#pragma once

#include "DeviceComponent.hpp"

class AbstractCleaningUnit : public DeviceComponent {
   protected:
    int power = 0;

   public:
    bool isOn() const override;
    void turnOn() override;
    void turnOff() override;

    virtual void clean(bool is_dust_detected) = 0;
    virtual void boostMode() = 0;
    virtual void normalMode() = 0;
};
