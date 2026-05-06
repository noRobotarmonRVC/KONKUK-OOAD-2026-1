#include "DriveMotor.hpp"

#include <iostream>

DriveMotor::DriveMotor(std::shared_ptr<AbstractNetwork> network)
    : m_network(std::move(network)) {}

void DriveMotor::moveForward() {
    std::cout << "[DriveMotor] move forward\n";
}

void DriveMotor::moveBackward() {
    std::cout << "[DriveMotor] move backward\n";
}

void DriveMotor::stop() {
    std::cout << "[DriveMotor] stop\n";
}

void DriveMotor::rotateRight() {
    std::cout << "[DriveMotor] rotate right\n";
}

void DriveMotor::rotateLeft() {
    std::cout << "[DriveMotor] rotate left\n";
}
