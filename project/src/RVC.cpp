#include "RVC.hpp"

#include "DriveMotor.hpp"
#include "DustSensor.hpp"
#include "ObstacleSensor.hpp"
#include "SweepingUnit.hpp"

#include <iostream>
#include <stdexcept>
#include <utility>

RVC::RVC()
    : cleaningController_(std::make_unique<CleaningController>(
          std::make_shared<DriveMotor>(),
          std::make_shared<SweepingUnit>(),
          std::make_shared<DustSensor>(),
          std::make_shared<ObstacleSensor>()
      )) {}

RVC::RVC(std::unique_ptr<CleaningController> cleaningController)
    : cleaningController_(std::move(cleaningController)) {
    if (!cleaningController_) {
        throw std::invalid_argument("RVC: cleaningController is null");
    }
}

void RVC::powerOn() {
    on_ = true;
    std::cout << "[RVC] power on\n";
}

void RVC::powerOff() {
    stopCleaning();
    on_ = false;
    std::cout << "[RVC] power off\n";
}

void RVC::startCleaning() {
    if (!on_) {
        std::cout << "[RVC] cannot start: power off\n";
        return;
    }

    cleaningController_->run();
}

void RVC::stopCleaning() {
    if (cleaningController_) {
        cleaningController_->stop();
    }
}
