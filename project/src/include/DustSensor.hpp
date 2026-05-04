#pragma once

#include "AbstractDustSensor.hpp"

class DustSensor : public AbstractDustSensor {
   public:
    DustSensor() = default;
    explicit DustSensor(bool is_dust_detected);

    bool isOn() const override;
    void turnOn() override;
    void turnOff() override;
    bool findDust() override;
};
