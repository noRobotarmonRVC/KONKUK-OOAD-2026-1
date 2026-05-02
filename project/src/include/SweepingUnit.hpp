#pragma once

#include "AbstractCleaningUnit.hpp"

class SweepingUnit : public AbstractCleaningUnit {
public:
    void clean(bool isDustDetected) override;
    void boostMode() override;
    void normalMode() override;
};
