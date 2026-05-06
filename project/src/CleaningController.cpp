#include "CleaningController.hpp"

#include <array>
#include <chrono>
#include <memory>
#include <stdexcept>
#include <thread>
#include <utility>

#include "DevicePowerManager.hpp"
#include "DriveController.hpp"
#include "SweepingController.hpp"

std::array<std::shared_ptr<DeviceComponent>, 3> CleaningController::makeDeviceArray(
    const std::shared_ptr<AbstractCleaningUnit>& cleaner,
    const std::shared_ptr<AbstractDustSensor>& dustSensor,
    const std::shared_ptr<AbstractObstacleSensor>& obstacleSensor
) {
    return {cleaner, dustSensor, obstacleSensor};
}

// 실제 실행용 생성자
CleaningController::CleaningController(
    std::shared_ptr<AbstractDriveMotor> motor,
    std::shared_ptr<AbstractCleaningUnit> cleaner,
    std::shared_ptr<AbstractDustSensor> dust_sensor,
    std::shared_ptr<AbstractObstacleSensor> obstacle_sensor
)
    : CleaningController(
          std::make_shared<DriveController>(motor),
          std::make_shared<SweepingController>(cleaner),
          std::make_shared<DevicePowerManager>(
              makeDeviceArray(cleaner, dust_sensor, obstacle_sensor)
          ),
          dust_sensor,
          obstacle_sensor
      ) {}

// Unit Test용 생성자
CleaningController::CleaningController(
    std::shared_ptr<AbstractDriveController> motor_controller,
    std::shared_ptr<AbstractSweepingController> sweeping_controller,
    std::shared_ptr<AbstractDevicePowerManager> device_controller,
    std::shared_ptr<AbstractDustSensor> dust_sensor,
    std::shared_ptr<AbstractObstacleSensor> obstacle_sensor
)
    : motor_controller(std::move(motor_controller)),
      sweeping_controller(std::move(sweeping_controller)),
      device_controller(std::move(device_controller)),
      dust_sensor(std::move(dust_sensor)),
      obstacle_sensor(std::move(obstacle_sensor)) {
    if (!this->motor_controller) {
        throw std::invalid_argument("CleaningController: motor_controller is null");
    }

    if (!this->sweeping_controller) {
        throw std::invalid_argument("CleaningController: sweeping_controller is null");
    }

    if (!this->device_controller) {
        throw std::invalid_argument("CleaningController: device_controller is null");
    }

    if (!this->dust_sensor) {
        throw std::invalid_argument("CleaningController: dust_sensor is null");
    }

    if (!this->obstacle_sensor) {
        throw std::invalid_argument("CleaningController: obstacle_sensor is null");
    }
}


void CleaningController::run() {
    if (!is_cleaning) {
        return;
    }

    is_cleaning = true;

    std::thread([this]() {
        while (is_cleaning) {
            std::array<int, 4> obstacleInfo = obstacle_sensor->findObstacle();

            if (obstacleInfo[0]) {
                sweeping_controller->turnOff();
                motor_controller->stop();

                if (obstacleInfo[0] &&
                    obstacleInfo[1] &&
                    obstacleInfo[2] &&
                    obstacleInfo[3]) {
                    stop();
                    return;
                }

                motor_controller->avoid(obstacleInfo);
                sweeping_controller->turnOn();
            }

            bool isDustDetected = dust_sensor->findDust();

            sweeping_controller->clean(isDustDetected);

            bool isCleanerOn = sweeping_controller->isOn();

            if (isCleanerOn) {
                motor_controller->moveForward();
            }

            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
            }

        sweeping_controller->turnOff();
        motor_controller->stop();
        dust_sensor->turnOff();
    }).detach();
}

void CleaningController::stop() {
    if (!is_cleaning) {
        return;
    }
    device_controller->allTurnOff();
    is_cleaning = false;
}

void CleaningController::turnOnDeviceComponents() {
    device_controller->allTurnOn();
}

void CleaningController::turnOffDeviceComponents() {
    device_controller->allTurnOff();
}

bool CleaningController::isCleaning() const {
    return is_cleaning;
}