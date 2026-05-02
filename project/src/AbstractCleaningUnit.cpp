#include "AbstractCleaningUnit.hpp"

#include <iostream>

bool AbstractCleaningUnit::isOn() const {
    return isOn_;
}

void AbstractCleaningUnit::turnOn() {
    isOn_ = true;
    std::cout << "[CleaningUnit] turn on\n";
}

void AbstractCleaningUnit::turnOff() {
    isOn_ = false;
    std::cout << "[CleaningUnit] turn off\n";
}
