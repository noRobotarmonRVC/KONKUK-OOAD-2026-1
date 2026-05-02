#include "RVC.hpp"

#include "CleaningController.hpp"
#include "DriveMotor.hpp"
#include "DustSensor.hpp"
#include "ObstacleSensor.hpp"
#include "SweepingUnit.hpp"

#include <memory>

int main() {
    auto motor = std::make_shared<DriveMotor>();
    auto cleaner = std::make_shared<SweepingUnit>();
    auto dustSensor = std::make_shared<DustSensor>();
    auto obstacleSensor = std::make_shared<ObstacleSensor>();

    dustSensor->setDustDetected(true);

    // index rule: [0] front, [1] right, [2] back, [3] left
    obstacleSensor->setObstacleInfo({1, 0, 0, 0});

    auto cleaningController = std::make_unique<CleaningController>(
        motor,
        cleaner,
        dustSensor,
        obstacleSensor
    );

    RVC rvc(std::move(cleaningController));

    rvc.powerOn();
    rvc.startCleaning();
    rvc.stopCleaning();
    rvc.powerOff();

    return 0;
}
