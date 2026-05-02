#pragma once

#include "IDustSensor.hpp"

class AbstractDustSensor : public IDustSensor {
protected:
    bool isDustDetected_ = false;

public:
    bool isOn() const override;
    void turnOn() override;
    void turnOff() override;
};
