#pragma once

#include "DeviceComponent.hpp"

class IDustSensor : public DeviceComponent {
public:
    ~IDustSensor() override = default;

    virtual bool findDust() = 0;
};
