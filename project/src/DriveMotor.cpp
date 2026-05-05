#include "DriveMotor.hpp"

#include <iostream>

DriveMotor::DriveMotor(SimulatorClient& client) : m_client(&client) {}

void DriveMotor::moveForward() {
    if (m_client) {
        m_client->sendAsync("MOVE_FORWARD");
    } else {
        throw std::invalid_argument("DriveMotor: client connection failed");
    }

    // 일단 주석 처리, test 필요 시 유지
    // std::cout << "[DriveMotor] move forward\n";
}

void DriveMotor::moveBackward() {
    if (m_client) {
        m_client->sendAsync("MOVE_BACKWARD");
    } else {
        throw std::invalid_argument("DriveMotor: client connection failed");
    }

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
