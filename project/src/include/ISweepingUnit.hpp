#pragma once

#include "DeviceComponent.hpp"

class ISweepingUnit : public DeviceComponent {
public:
    ~ISweepingUnit() override = default;

    virtual void clean(bool isDustDetected) = 0;
    virtual void boostMode() = 0;
    virtual void normalMode() = 0;
};
