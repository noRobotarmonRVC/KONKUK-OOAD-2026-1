#include "CleaningController.hpp"
#include <thread>
#include <stdexcept>
#include <utility>
#include <array>
#include <chrono>

std::array<std::shared_ptr<DeviceComponent>, 3> CleaningController::makeDeviceArray(
    const std::shared_ptr<AbstractCleaningUnit>& cleaner,
    const std::shared_ptr<AbstractDustSensor>& dustSensor,
    const std::shared_ptr<AbstractObstacleSensor>& obstacleSensor
) {
    return { cleaner, dustSensor, obstacleSensor };
}

CleaningController::CleaningController(
    std::shared_ptr<AbstractDriveMotor> motor,
    std::shared_ptr<AbstractCleaningUnit> cleaner,
    std::shared_ptr<AbstractDustSensor> dust_sensor,
    std::shared_ptr<AbstractObstacleSensor> obstacle_sensor
)
    : motor_controller(motor),
      sweeping_controller(cleaner),
      device_controller(makeDeviceArray(cleaner, dust_sensor, obstacle_sensor)),
      dust_sensor(dust_sensor),
      obstacle_sensor(obstacle_sensor) {

    if (!motor) {
        throw std::invalid_argument("CleaningController: motor is null");
    }

    if (!cleaner) {
        throw std::invalid_argument("CleaningController: cleaner is null");
    }

    if (!this->dust_sensor || !this->obstacle_sensor) {
        throw std::invalid_argument("CleaningController: sensor is null");
    }
}

void CleaningController::run() {
    if (is_cleaning) {
        return;
    }

    is_cleaning = true;

    std::thread([this]() {
        while (is_cleaning) {
            std::array<int, 4> obstacleInfo = obstacle_sensor->findObstacle();

            if (obstacleInfo[0]) {
                sweeping_controller.turnOff();
                motor_controller.stop();

                if (obstacleInfo[0] &&
                    obstacleInfo[1] &&
                    obstacleInfo[2] &&
                    obstacleInfo[3]) {
                    stop();
                    return;
                }

                motor_controller.avoid(obstacleInfo);
                sweeping_controller.turnOn();
            }

            bool isDustDetected = dust_sensor->findDust();

            sweeping_controller.clean(isDustDetected);

            bool isCleanerOn = sweeping_controller.isOn();

            if (isCleanerOn) {
                motor_controller.moveForward();
            }

            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        }

        sweeping_controller.turnOff();
        motor_controller.stop();
        dust_sensor->turnOff();
    }).detach();
}

void CleaningController::stop() {
    if (!is_cleaning) {
        return;
    }

    motor_controller.stop();
    device_controller.allTurnOff();
    is_cleaning = false;
}

void CleaningController::turnOnDeviceComponents() {
    device_controller.allTurnOn();
}