#pragma once

#include "IDriveMotor.hpp"

class AbstractDriveMotor : public IDriveMotor {
public:
    ~AbstractDriveMotor() override = default;
};
