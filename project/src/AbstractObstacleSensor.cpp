#include "AbstractObstacleSensor.hpp"

#include <iostream>

bool AbstractObstacleSensor::isOn() const {
    return is_on;
}

void AbstractObstacleSensor::turnOn() {
    is_on = true;
    std::cout << "[ObstacleSensor] turn on\n";
}

void AbstractObstacleSensor::turnOff() {
    is_on = false;
    std::cout << "[ObstacleSensor] turn off\n";
}
