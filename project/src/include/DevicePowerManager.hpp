#pragma once

#include "DeviceComponent.hpp"

#include <array>
#include <memory>

class DevicePowerManager {
private:
    std::array<std::shared_ptr<DeviceComponent>, 3> devices;

public:
    explicit DevicePowerManager(
        const std::array<std::shared_ptr<DeviceComponent>, 3>& devices
    );

    void allTurnOn();
    void allTurnOff();
};
