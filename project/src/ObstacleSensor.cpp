#include "ObstacleSensor.hpp"

#include <iostream>
#include <sstream>
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
        // std::cout << "[ObstacleSensor] cannot find obstacle: power off\n";
        return {0, 0, 0, 0};
    }

    // std::cout << "[ObstacleSensor] obstacle info: ["
    //           << obstacle_info[0] << ", "
    //           << obstacle_info[1] << ", "
    //           << obstacle_info[2] << ", "
    //           << obstacle_info[3] << "]\n";

    std::string response = m_network->request("FIND_OBSTACLE");
    std::istringstream iss(response);

    std::string prefix;
    int front = 0;
    int right = 0;
    int back = 0;
    int left = 0;

    if (!(iss >> prefix >> front >> right >> back >> left)) {
        return {0, 0, 0, 0};
    }

    if (prefix != "OBSTACLE") {
        return {0, 0, 0, 0};
    }

    obstacle_info[0] = front ? 1 : 0;
    obstacle_info[1] = right ? 1 : 0;
    obstacle_info[2] = back ? 1 : 0;
    obstacle_info[3] = left ? 1 : 0;

    return obstacle_info;
}
