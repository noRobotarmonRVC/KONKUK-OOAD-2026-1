#include "ObstacleSensor.hpp"

#include <iostream>

ObstacleSensor::ObstacleSensor(const std::array<int, 4>& initialObstacleInfo) {
    obstacleInfo_ = initialObstacleInfo;
}

std::array<int, 4> ObstacleSensor::findObstacle() {
    if (!isOn()) {
        std::cout << "[ObstacleSensor] cannot find obstacle: power off\n";
        return {0, 0, 0, 0};
    }

    std::cout << "[ObstacleSensor] obstacle info: ["
              << obstacleInfo_[0] << ", "
              << obstacleInfo_[1] << ", "
              << obstacleInfo_[2] << ", "
              << obstacleInfo_[3] << "]\n";

    return obstacleInfo_;
}

void ObstacleSensor::setObstacleInfo(const std::array<int, 4>& obstacleInfo) {
    obstacleInfo_ = obstacleInfo;
}
