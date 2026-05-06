#include "DriveMotor.hpp"

#include <iostream>

DriveMotor::DriveMotor(std::shared_ptr<AbstractNetwork> network)
    : m_network(std::move(network)) {}

void DriveMotor::moveForward() {
    if (m_client) {
        m_client->sendAsync("MOVE_FORWARD");
    }

    // 일단 주석 처리, test 필요 시 유지
    // std::cout << "[DriveMotor] move forward\n";
}

void DriveMotor::moveBackward() {
    if (m_client) {
        m_client->sendAsync("MOVE_BACKWARD");
    }

    // std::cout << "[DriveMotor] move backward\n";
}

void DriveMotor::stop() {
    if (m_client) {
        m_client->sendAsync("STOP_MOTOR");
    }

    // std::cout << "[DriveMotor] stop\n";
}

void DriveMotor::rotateRight() {
    if (m_client) {
        m_client->sendAsync("ROTATE_RIGHT");
    }

    // std::cout << "[DriveMotor] rotate right\n";
}

void DriveMotor::rotateLeft() {
    if (m_client) {
        m_client->sendAsync("ROTATE_LEFT");
    }

    // std::cout << "[DriveMotor] rotate left\n";
}
