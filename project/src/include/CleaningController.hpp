#include <array>
#include <memory>

#include "AbstractCleaningController.hpp"
#include "AbstractCleaningUnit.hpp"
#include "AbstractDevicePowerManager.hpp"
#include "AbstractDriveController.hpp"
#include "AbstractDriveMotor.hpp"
#include "AbstractDustSensor.hpp"
#include "AbstractObstacleSensor.hpp"
#include "AbstractSweepingController.hpp"
#include "DeviceComponent.hpp"

class CleaningController : public AbstractCleaningController {
private:
    std::shared_ptr<AbstractDriveController> motor_controller;
    std::shared_ptr<AbstractSweepingController> sweeping_controller;
    std::shared_ptr<AbstractDevicePowerManager> device_controller;

    std::shared_ptr<AbstractDustSensor> dust_sensor;
    std::shared_ptr<AbstractObstacleSensor> obstacle_sensor;

    bool is_cleaning = false;

    static std::array<std::shared_ptr<DeviceComponent>, 3> makeDeviceArray(
        const std::shared_ptr<AbstractCleaningUnit>& cleaner,
        const std::shared_ptr<AbstractDustSensor>& dust_sensor,
        const std::shared_ptr<AbstractObstacleSensor>& obstacle_sensor
    );

public:
    CleaningController(
        std::shared_ptr<AbstractDriveController> motor_controller,
        std::shared_ptr<AbstractSweepingController> sweeping_controller,
        std::shared_ptr<AbstractDevicePowerManager> device_controller,
        std::shared_ptr<AbstractDustSensor> dust_sensor,
        std::shared_ptr<AbstractObstacleSensor> obstacle_sensor
    );
    void run() override;
    void stop() override;

    void turnOnDeviceComponents() override;
    void turnOffDeviceComponents() override;

    bool isCleaning() const override;
};