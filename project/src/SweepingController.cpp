#include "SweepingController.hpp"

#include <stdexcept>
#include <utility>

SweepingController::SweepingController(std::shared_ptr<ISweepingUnit> cleaner)
    : cleaner_(std::move(cleaner)) {
    if (!cleaner_) {
        throw std::invalid_argument("SweepingController: cleaner is null");
    }
}

void SweepingController::turnOn() {
    cleaner_->turnOn();
}

void SweepingController::turnOff() {
    cleaner_->turnOff();
}

bool SweepingController::isOn() const {
    return cleaner_->isOn();
}

void SweepingController::clean(bool isDustDetected) {
    cleaner_->clean(isDustDetected);
}
