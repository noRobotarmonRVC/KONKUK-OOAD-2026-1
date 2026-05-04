#pragma once

#include "AbstractCleaningUnit.hpp"

class SweepingUnit : public AbstractCleaningUnit {
   public:
    bool isOn() const override;
    void turnOn() override;
    void turnOff() override;
    void clean(bool is_dust_detected) override;
    void boostMode() override;
    void normalMode() override;
};
