#include <memory>

#include "AbstractNetwork.hpp"
#include "CleaningController.hpp"
#include "DriveMotor.hpp"
#include "DustSensor.hpp"
#include "ObstacleSensor.hpp"
#include "RVC.hpp"
#include "SweepingUnit.hpp"
#include "TCPNetwork.hpp"

int main() {
    auto network = std::make_shared<TCPNetwork>("127.0.0.1", 9000);

    auto motor = std::make_shared<DriveMotor>(network);
    auto cleaner = std::make_shared<SweepingUnit>(network);
    auto dustSensor = std::make_shared<DustSensor>(network);
    auto obstacleSensor = std::make_shared<ObstacleSensor>(network);

    // dustSensor->setDustDetected(true);

    // index rule: [0] front, [1] right, [2] back, [3] left
    // obstacleSensor->setObstacleInfo({1, 0, 0, 0});

    auto cleaningController = std::make_unique<CleaningController>(
        motor,
        cleaner,
        dustSensor,
        obstacleSensor);

    RVC rvc(std::move(cleaningController));

    rvc.powerOn();
    rvc.startCleaning();
    rvc.stopCleaning();
    rvc.powerOff();

    return 0;
}
