#include "AbstractDustSensor.hpp"

#include <iostream>

bool AbstractDustSensor::isOn() const {
    return isOn_;
}

void AbstractDustSensor::turnOn() {
    isOn_ = true;
    std::cout << "[DustSensor] turn on\n";
}

void AbstractDustSensor::turnOff() {
    isOn_ = false;
    std::cout << "[DustSensor] turn off\n";
}
