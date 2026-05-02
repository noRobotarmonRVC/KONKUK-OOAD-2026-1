#pragma once

#include "DeviceComponent.hpp"
#include "DevicePowerManager.hpp"
#include "DriveController.hpp"
#include "IDriveMotor.hpp"
#include "IDustSensor.hpp"
#include "IObstacleSensor.hpp"
#include "ISweepingUnit.hpp"
#include "SweepingController.hpp"

#include <array>
#include <memory>

class CleaningController {
private:
    DriveController motorController_;
    SweepingController cleanerController_;
    DevicePowerManager deviceController_;

    std::shared_ptr<IDustSensor> dustSensor_;
    std::shared_ptr<IObstacleSensor> obstacleSensor_;

    bool isCleaning_ = false;

    static std::array<std::shared_ptr<DeviceComponent>, 3> makeDeviceArray(
        const std::shared_ptr<ISweepingUnit>& cleaner,
        const std::shared_ptr<IDustSensor>& dustSensor,
        const std::shared_ptr<IObstacleSensor>& obstacleSensor
    );

public:
    CleaningController(
        std::shared_ptr<IDriveMotor> motor,
        std::shared_ptr<ISweepingUnit> cleaner,
        std::shared_ptr<IDustSensor> dustSensor,
        std::shared_ptr<IObstacleSensor> obstacleSensor
    );

    void run();
    void stop();
};
