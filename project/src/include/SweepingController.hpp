#pragma once

#include "ISweepingUnit.hpp"

#include <memory>

class SweepingController {
private:
    std::shared_ptr<ISweepingUnit> cleaner_;

public:
    explicit SweepingController(std::shared_ptr<ISweepingUnit> cleaner);

    void turnOn();
    void turnOff();
    bool isOn() const;
    void clean(bool isDustDetected);
};
