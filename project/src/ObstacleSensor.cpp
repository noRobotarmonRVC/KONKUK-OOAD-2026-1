#include "ObstacleSensor.hpp"

#include <iostream>
#include <sstream>
ObstacleSensor::ObstacleSensor(std::shared_ptr<AbstractNetwork> network)
    : m_network(std::move(network)) {}

bool ObstacleSensor::isOn() const { return is_on; 
}
void ObstacleSensor::turnOn() { is_on = true; 
 m_network->request("OBSTACLE_SENSOR_ON");}
void ObstacleSensor::turnOff() { is_on = false; 
 m_network->request("OBSTACLE_SENSOR_OFF");}

ObstacleSensor::ObstacleSensor(const std::array<int, 2>& initialObstacleInfo) {
    obstacle_info = initialObstacleInfo;
}

std::array<int, 2> ObstacleSensor::findObstacle() {
    std::cout << "[ObstacleSensor] findObstacle called\n";
    if (!isOn()) {
        // std::cout << "[ObstacleSensor] cannot find obstacle: power off\n";
        return {0, 0};
    }

    // std::cout << "[ObstacleSensor] obstacle info: ["
    //           << obstacle_info[0] << ", "
    //           << obstacle_info[1] << "]\n";
    std::string response = m_network->request("FIND_OBSTACLE");
    std::istringstream iss(response);

    std::string prefix;
    int front = 0;
    int right = 0;
    int back = 0;
    int left = 0;
    std::cout << "[ObstacleSensor] Network response: " << response << std::endl;

    if (!(iss >> prefix >> front >> left >> back >> right)) {
        return {0, 0};
    }

    if (prefix != "OBSTACLE") {
        return {0, 0};
    }

    obstacle_info[0] = front ? 1 : 0;
    obstacle_info[1] = left ? 1 : 0;
    // obstacle_info[2] = back ? 1 : 0;
    // obstacle_info[3] = left ? 1 : 0;
    
    std::cout << "[ObstacleSensor] obstacle info: ["
              << obstacle_info[0] << ", "
              << obstacle_info[1] << "]\n";

    return obstacle_info;
}
