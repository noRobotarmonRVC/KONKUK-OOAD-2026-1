#pragma once

#include <memory>

#include "AbstractCleaningUnit.hpp"
#include "AbstractSweepingController.hpp"

class SweepingController : public AbstractSweepingController {
private:
    std::shared_ptr<AbstractCleaningUnit> cleaner;

public:
    explicit SweepingController(std::shared_ptr<AbstractCleaningUnit> cleaner);

    void turnOn() override;
    void turnOff() override;
    bool isOn() const override;

    void clean(bool isDustDetected) override;
};