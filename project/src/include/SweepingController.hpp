#pragma once

#include <memory>

#include "AbstractCleaningUnit.hpp"

class SweepingController {
   private:
    std::shared_ptr<AbstractCleaningUnit> cleaner;

   public:
    explicit SweepingController(std::shared_ptr<AbstractCleaningUnit> cleaner);

    void turnOn();
    void turnOff();
    bool isOn() const;
    void clean(bool is_dust_detected);
};
