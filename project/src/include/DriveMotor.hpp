#pragma once

#include "AbstractDriveMotor.hpp"
#include "SimulatorClient.hpp"

class DriveMotor : public AbstractDriveMotor {
public:
    DriveMotor() = default;
    explicit DriveMotor(SimulatorClient& client);

    void moveForward() override;
    void moveBackward() override;
    void stop() override;
    void rotateRight() override;
    void rotateLeft() override;

private:
    SimulatorClient* m_client = nullptr;
};
