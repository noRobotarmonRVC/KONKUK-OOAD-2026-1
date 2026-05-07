#pragma once

class AbstractSweepingController {
public:
    virtual ~AbstractSweepingController() = default;

    virtual void turnOn() = 0;
    virtual void turnOff() = 0;

    virtual bool isOn() const = 0;

    virtual void clean(bool isDustDetected) = 0;
};