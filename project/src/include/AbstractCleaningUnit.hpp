#pragma once

#include "ISweepingUnit.hpp"

class AbstractCleaningUnit : public ISweepingUnit {
protected:
    int power_ = 0;

public:
    bool isOn() const override;
    void turnOn() override;
    void turnOff() override;
};
