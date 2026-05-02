#include "DustSensor.hpp"

#include <iostream>

DustSensor::DustSensor(bool initialDustDetected) {
    isDustDetected_ = initialDustDetected;
}

bool DustSensor::findDust() {
    if (!isOn()) {
        std::cout << "[DustSensor] cannot find dust: power off\n";
        return false;
    }

    std::cout << "[DustSensor] dust detected: "
              << (isDustDetected_ ? "true" : "false") << '\n';

    return isDustDetected_;
}

void DustSensor::setDustDetected(bool detected) {
    isDustDetected_ = detected;
}
