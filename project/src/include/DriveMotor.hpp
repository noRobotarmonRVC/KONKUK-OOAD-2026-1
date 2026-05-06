#pragma once

#include <memory>

#include "AbstractDriveMotor.hpp"
#include "AbstractNetwork.hpp"

class DriveMotor : public AbstractDriveMotor {
   public:
    DriveMotor() = default;
    explicit DriveMotor(std::shared_ptr<AbstractNetwork> network);

    void moveForward() override;
    void moveBackward() override;
    void stop() override;
    void rotateRight() override;
    void rotateLeft() override;

   private:
    std::shared_ptr<AbstractNetwork> m_network = nullptr;
};
