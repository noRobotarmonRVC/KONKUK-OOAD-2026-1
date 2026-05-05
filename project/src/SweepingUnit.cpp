#include "SweepingUnit.hpp"

#include <iostream>

SweepingUnit::SweepingUnit(SimulatorClient& client) : m_client(&client) {}

bool SweepingUnit::isOn() const {
    return is_on;
}
void SweepingUnit::turnOn() {
    m_client->sendAsync("CLEANER_ON");
    is_on = true;
}
void SweepingUnit::turnOff() {
    m_client->sendAsync("CLEANER_OFF");
    is_on = false;
}

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
    power = 2;
    m_client->sendAsync("BOOST_MODE");
    std::cout << "[SweepingUnit] boost mode\n";
}

void SweepingUnit::normalMode() {
    power = 1;
    m_client->sendAsync("NORMAL_MODE");
    std::cout << "[SweepingUnit] normal mode\n";
}
