#pragma once

#include "AbstractDustSensor.hpp"

class DustSensor : public AbstractDustSensor {
public:
    DustSensor() = default;
    explicit DustSensor(bool initialDustDetected);

    bool findDust() override;
    void setDustDetected(bool detected);
};
