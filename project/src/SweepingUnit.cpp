#include "SweepingUnit.hpp"

#include <iostream>

void SweepingUnit::clean(bool isDustDetected) {
    if (!isOn()) {
        std::cout << "[SweepingUnit] cannot clean: power off\n";
        return;
    }

    if (isDustDetected) {
        boostMode();
    } else {
        normalMode();
    }

    std::cout << "[SweepingUnit] clean with power " << power << '\n';
}

void SweepingUnit::boostMode() {
    power = 100;
    std::cout << "[SweepingUnit] boost mode\n";
}

void SweepingUnit::normalMode() {
    power = 50;
    std::cout << "[SweepingUnit] normal mode\n";
}
