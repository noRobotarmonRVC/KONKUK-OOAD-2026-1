#include "AbstractDustSensor.hpp"

#include <iostream>

bool AbstractDustSensor::isOn() const {
    return is_on;
}

void AbstractDustSensor::turnOn() {
    is_on = true;
    // std::cout << "[DustSensor] turn on\n";
}

void AbstractDustSensor::turnOff() {
    is_on = false;
    // std::cout << "[DustSensor] turn off\n";
}
