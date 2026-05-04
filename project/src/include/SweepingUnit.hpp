#pragma once

#include "AbstractCleaningUnit.hpp"

class SweepingUnit : public AbstractCleaningUnit {
   public:
    void clean(bool is_dust_detected) override;
    void boostMode() override;
    void normalMode() override;
};
