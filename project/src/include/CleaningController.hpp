#pragma once

#include <array>
#include <memory>

#include "AbstractCleaningUnit.hpp"
#include "AbstractDriveMotor.hpp"
#include "AbstractDustSensor.hpp"
#include "AbstractObstacleSensor.hpp"
#include "DeviceComponent.hpp"
#include "DevicePowerManager.hpp"
#include "DriveController.hpp"
#include "SweepingController.hpp"

class CleaningController {
   private:
    DriveController motor_controller;
    SweepingController sweeping_controller;
    DevicePowerManager device_controller;

    std::shared_ptr<AbstractDustSensor> dust_sensor;
    std::shared_ptr<AbstractObstacleSensor> obstacle_sensor;

    bool is_cleaning = false;

    static std::array<std::shared_ptr<DeviceComponent>, 3> makeDeviceArray(
        const std::shared_ptr<AbstractCleaningUnit>& cleaner,
        const std::shared_ptr<AbstractDustSensor>& dust_sensor,
        const std::shared_ptr<AbstractObstacleSensor>& obstacle_sensor);

   public:
    CleaningController(
        std::shared_ptr<AbstractDriveMotor> motor,
        std::shared_ptr<AbstractCleaningUnit> cleaner,
        std::shared_ptr<AbstractDustSensor> dust_sensor,
        std::shared_ptr<AbstractObstacleSensor> obstacle_sensor);

    void run();
    void stop();
    void turnOnDeviceComponents();
};
