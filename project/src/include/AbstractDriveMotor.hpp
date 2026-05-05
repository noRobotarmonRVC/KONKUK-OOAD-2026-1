#pragma once

class AbstractDriveMotor {
   public:
    virtual void moveForward() = 0;
    virtual void moveBackward() = 0;
    virtual void stop() = 0;
    virtual void rotateRight() = 0;
    virtual void rotateLeft() = 0;
};
