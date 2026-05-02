#include "AbstractObstacleSensor.hpp"

#include <iostream>

bool AbstractObstacleSensor::isOn() const {
    return isOn_;
}

void AbstractObstacleSensor::turnOn() {
    isOn_ = true;
    std::cout << "[ObstacleSensor] turn on\n";
}

void AbstractObstacleSensor::turnOff() {
    isOn_ = false;
    std::cout << "[ObstacleSensor] turn off\n";
}
