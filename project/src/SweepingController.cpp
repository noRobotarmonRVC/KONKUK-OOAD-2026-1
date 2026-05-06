#include "SweepingController.hpp"

#include <stdexcept>
#include <utility>

SweepingController::SweepingController(std::shared_ptr<AbstractCleaningUnit> cleaner)
    : cleaner(std::move(cleaner)) {
    if (!this->cleaner) {
        throw std::invalid_argument("SweepingController: cleaner is null");
    }
}

void SweepingController::turnOn() {
    cleaner->turnOn();
}

void SweepingController::turnOff() {
    cleaner->turnOff();
}

bool SweepingController::isOn() const {
    return cleaner->isOn();
}

void SweepingController::clean(bool isDustDetected) {
    cleaner->clean(isDustDetected);
}
