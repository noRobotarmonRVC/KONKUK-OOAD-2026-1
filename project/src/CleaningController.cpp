#include "CleaningController.hpp"

#include <stdexcept>
#include <utility>

std::array<std::shared_ptr<DeviceComponent>, 3> CleaningController::makeDeviceArray(
    const std::shared_ptr<ISweepingUnit>& cleaner,
    const std::shared_ptr<IDustSensor>& dustSensor,
    const std::shared_ptr<IObstacleSensor>& obstacleSensor
) {
    return {cleaner, dustSensor, obstacleSensor};
}

CleaningController::CleaningController(
    std::shared_ptr<IDriveMotor> motor,
    std::shared_ptr<ISweepingUnit> cleaner,
    std::shared_ptr<IDustSensor> dustSensor,
    std::shared_ptr<IObstacleSensor> obstacleSensor
)
    : motorController_(std::move(motor)),
      cleanerController_(cleaner),
      deviceController_(makeDeviceArray(cleaner, dustSensor, obstacleSensor)),
      dustSensor_(std::move(dustSensor)),
      obstacleSensor_(std::move(obstacleSensor)) {
    if (!dustSensor_ || !obstacleSensor_) {
        throw std::invalid_argument("CleaningController: sensor is null");
    }
}

void CleaningController::run() {
    if (!isCleaning_) {
        isCleaning_ = true;
        deviceController_.allTurnOn();
    }

    const std::array<int, 4> obstacleInfo = obstacleSensor_->findObstacle();
    motorController_.avoid(obstacleInfo);

    const bool isDustDetected = dustSensor_->findDust();
    cleanerController_.clean(isDustDetected);
}

void CleaningController::stop() {
    if (!isCleaning_) {
        return;
    }

    motorController_.stop();
    deviceController_.allTurnOff();
    isCleaning_ = false;
}
