#include "ObstacleSensor.hpp"

#include <iostream>

ObstacleSensor::ObstacleSensor(std::shared_ptr<AbstractNetwork> network)
    : m_network(std::move(network)) {}

bool ObstacleSensor::isOn() const { return is_on; }
void ObstacleSensor::turnOn() { is_on = true; }
void ObstacleSensor::turnOff() { is_on = false; }

ObstacleSensor::ObstacleSensor(const std::array<int, 4>& initialObstacleInfo) {
    obstacle_info = initialObstacleInfo;
}

std::array<int, 4> ObstacleSensor::findObstacle() {
    if (!isOn()) {
        std::cout << "[ObstacleSensor] cannot find obstacle: power off\n";
        return {0, 0, 0, 0};
    }

    std::cout << "[ObstacleSensor] obstacle info: ["
              << obstacle_info[0] << ", "
              << obstacle_info[1] << ", "
              << obstacle_info[2] << ", "
              << obstacle_info[3] << "]\n";

    return obstacle_info;
}
