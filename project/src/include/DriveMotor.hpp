#pragma once

#include "AbstractDriveMotor.hpp"

class DriveMotor : public AbstractDriveMotor {
public:
    void moveForward() override;
    void moveBackward() override;
    void stop() override;
    void rotateRight() override;
    void rotateLeft() override;
};
