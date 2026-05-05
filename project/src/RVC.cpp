#include "RVC.hpp"

#include <iostream>
#include <stdexcept>
#include <utility>

#include "DriveMotor.hpp"
#include "DustSensor.hpp"
#include "ObstacleSensor.hpp"
#include "SweepingUnit.hpp"

RVC::RVC()
    : cleaning_controller(std::make_unique<CleaningController>(
          std::make_shared<DriveMotor>(),
          std::make_shared<SweepingUnit>(),
          std::make_shared<DustSensor>(),
          std::make_shared<ObstacleSensor>())) {}

RVC::RVC(std::shared_ptr<CleaningController> cleaningController)
    : cleaning_controller(std::move(cleaningController)) {
    if (!cleaning_controller) {
        throw std::invalid_argument("RVC: cleaningController is null");
    }
}

void RVC::powerOn() {
    if(!on){
        on = true;
        // std::cout << "[RVC] power on\n";
        cleaning_controller->turnOnDeviceComponents();
    }
}

void RVC::powerOff() {
    if(on){
        stopCleaning();
        on = false;
        // std::cout << "[RVC] power off\n";
    }
}

void RVC::startCleaning() {
    if (!on) {
        // std::cout << "[RVC] cannot start: power off\n";
        return;
    }
    cleaning_controller->run();
}

void RVC::stopCleaning() {
    if (cleaning_controller) {
        cleaning_controller->stop();
    }
}
