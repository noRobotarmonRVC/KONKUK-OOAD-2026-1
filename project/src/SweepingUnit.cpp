#include "SweepingUnit.hpp"

#include <iostream>

SweepingUnit::SweepingUnit(std::shared_ptr<AbstractNetwork> network)
    : m_network(std::move(network)) {}

bool SweepingUnit::isOn() const { return is_on; }
void SweepingUnit::turnOn() { is_on = true; }
void SweepingUnit::turnOff() { is_on = false; }

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
