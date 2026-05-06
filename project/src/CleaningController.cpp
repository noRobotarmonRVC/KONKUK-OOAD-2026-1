#include "CleaningController.hpp"

#include <stdexcept>
#include <utility>

std::array<std::shared_ptr<DeviceComponent>, 3> CleaningController::makeDeviceArray(
    const std::shared_ptr<AbstractCleaningUnit>& cleaner,
    const std::shared_ptr<AbstractDustSensor>& dustSensor,
    const std::shared_ptr<AbstractObstacleSensor>& obstacleSensor) {
    return {cleaner, dustSensor, obstacleSensor};
}

CleaningController::CleaningController(
    std::shared_ptr<AbstractDriveMotor> motor,
    std::shared_ptr<AbstractCleaningUnit> cleaner,
    std::shared_ptr<AbstractDustSensor> dustSensor,
    std::shared_ptr<AbstractObstacleSensor> obstacleSensor)
    : motor_controller(std::move(motor)),
      sweeping_controller(cleaner),
      device_controller(makeDeviceArray(cleaner, dustSensor, obstacleSensor)),
      dust_sensor(std::move(dustSensor)),
      obstacle_sensor(std::move(obstacleSensor)) {
    if (!this->dust_sensor || !this->obstacle_sensor) {
        throw std::invalid_argument("CleaningController: sensor is null");
    }
}

void CleaningController::run() {
    if (!is_cleaning) {
        is_cleaning = true;
        device_controller.allTurnOn();
    }

    const std::array<int, 4> obstacleInfo = obstacle_sensor->findObstacle();
    motor_controller.avoid(obstacleInfo);

    const bool isDustDetected = dust_sensor->findDust();
    sweeping_controller.clean(isDustDetected);
}

void CleaningController::stop() {
    if (!is_cleaning) {
        return;
    }

    motor_controller.stop();
    device_controller.allTurnOff();
    is_cleaning = false;
}
