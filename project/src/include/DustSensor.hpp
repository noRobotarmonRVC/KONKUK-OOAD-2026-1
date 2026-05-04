#pragma once

#include "AbstractDustSensor.hpp"

class DustSensor : public AbstractDustSensor {
   public:
    DustSensor() = default;
    explicit DustSensor(bool is_dust_detected);

    bool findDust() override;
    void setDustDetected(bool detected);
};
