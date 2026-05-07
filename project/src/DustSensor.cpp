#include "DustSensor.hpp"

#include <iostream>
#include <string>
DustSensor::DustSensor(std::shared_ptr<AbstractNetwork> network)
    : m_network(std::move(network)) {}

bool DustSensor::isOn() const { return is_on; }
void DustSensor::turnOn() { is_on = true; 
    m_network->request("DUST_SENSOR_ON");
}
void DustSensor::turnOff() { is_on = false;
    m_network->request("DUST_SENSOR_OFF");
}

DustSensor::DustSensor(bool initialDustDetected) {
    is_dust_detected = initialDustDetected;
}

bool DustSensor::findDust() {
    if (!isOn()) {
        // std::cout << "[DustSensor] cannot find dust: power off\n";
        return false;
    }

    // std::cout << "[DustSensor] dust detected: "
    //           << (is_dust_detected ? "true" : "false") << '\n';
    std::string response = m_network->request("FIND_DUST");
    is_dust_detected = (response == "DUST 1");
    return is_dust_detected;
}