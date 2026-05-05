#include "DevicePowerManager.hpp"

#include <stdexcept>

DevicePowerManager::DevicePowerManager(
    const std::array<std::shared_ptr<DeviceComponent>, 3>& devices
) : devices(devices) {
    for (const auto& device : devices) {
        if (!device) {
            throw std::invalid_argument("DevicePowerManager: device is null");
        }
    }
}

void DevicePowerManager::allTurnOn() {
    for (const auto& device : devices) {
        device->turnOn();
    }
}

void DevicePowerManager::allTurnOff() {
    for (const auto& device : devices) {
        device->turnOff();
    }
}
