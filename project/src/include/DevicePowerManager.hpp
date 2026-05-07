#pragma once

#include <array>
#include <memory>

#include "AbstractDevicePowerManager.hpp"
#include "DeviceComponent.hpp"

class DevicePowerManager : public AbstractDevicePowerManager {
private:
    std::array<std::shared_ptr<DeviceComponent>, 3> devices;

public:
    explicit DevicePowerManager(
        const std::array<std::shared_ptr<DeviceComponent>, 3>& devices
    );

    void allTurnOn() override;
    void allTurnOff() override;
};