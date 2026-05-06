#include "DustSensor.hpp"

#include <iostream>

DustSensor::DustSensor(std::shared_ptr<AbstractNetwork> network)
    : m_network(std::move(network)) {}

bool DustSensor::isOn() const { return is_on; }
void DustSensor::turnOn() { is_on = true; }
void DustSensor::turnOff() { is_on = false; }

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