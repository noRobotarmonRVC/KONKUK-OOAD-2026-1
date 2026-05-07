#include "SweepingUnit.hpp"

#include <iostream>

SweepingUnit::SweepingUnit(std::shared_ptr<AbstractNetwork> network)
    : m_network(std::move(network)) {}

bool SweepingUnit::isOn() const { return is_on; }
void SweepingUnit::turnOn() { is_on = true; }
void SweepingUnit::turnOff() { is_on = false; }

void SweepingUnit::clean(bool is_dust_detected) {
    if (!isOn()) {
        std::cout << "[SweepingUnit] cannot clean: power off\n";
        return;
    }

    if (is_dust_detected) {
        boostMode();
    } else {
        normalMode();
    }

    std::cout << "[SweepingUnit] clean with power " << power << '\n';
}

void SweepingUnit::boostMode() {
    m_network->send("BOOST_MODE");
    power = 2;
    std::cout << "[SweepingUnit] boost mode\n";
}


void SweepingUnit::normalMode() {
    m_network->send("NORMAL_MODE");
    power = 1;
    std::cout << "[SweepingUnit] normal mode\n";
}
