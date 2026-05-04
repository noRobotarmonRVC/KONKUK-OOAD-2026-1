#include "DustSensor.hpp"

#include <iostream>

DustSensor::DustSensor(bool initialDustDetected) {
    is_dust_detected = initialDustDetected;
}

bool DustSensor::findDust() {
    if (!isOn()) {
        std::cout << "[DustSensor] cannot find dust: power off\n";
        return false;
    }

    std::cout << "[DustSensor] dust detected: "
              << (is_dust_detected ? "true" : "false") << '\n';

    return is_dust_detected;
}