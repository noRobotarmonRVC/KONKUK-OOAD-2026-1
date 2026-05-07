#pragma once

class AbstractDevicePowerManager {
public:
    virtual ~AbstractDevicePowerManager() = default;

    virtual void allTurnOn() = 0;
    virtual void allTurnOff() = 0;
};