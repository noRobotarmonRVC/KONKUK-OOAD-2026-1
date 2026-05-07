#include "DevicePowerManager.hpp"

#include <utility>

DevicePowerManager::DevicePowerManager(
    const std::array<std::shared_ptr<DeviceComponent>, 3>& devices
)
    : devices(devices) {}

void DevicePowerManager::allTurnOn() {
    for (const auto& device : devices) {
        if (device) {
            device->turnOn();
        }
    }
}

void DevicePowerManager::allTurnOff() {
    for (const auto& device : devices) {
        if (device) {
            device->turnOff();
        }
    }
}