#include "RVC.hpp"

#include <stdexcept>
#include <utility>

RVC::RVC(std::shared_ptr<AbstractCleaningController> cleaningController)
    : cleaningController(std::move(cleaningController)) {
    if (!this->cleaningController) {
        throw std::invalid_argument("RVC: cleaningController is null");
    }
}

void RVC::powerOn() {
    if (on) {
        return;
    }

    on = true;
    cleaningController->turnOnDeviceComponents();
}

void RVC::powerOff() {
    if (!on) {
        return;
    }

    stopCleaning();
    cleaningController->turnOffDeviceComponents();
    on = false;
}

void RVC::startCleaning() {
    if (!on) {
        return;
    }

    cleaningController->run();
}

void RVC::stopCleaning() {
    cleaningController->stop();
}

bool RVC::isOn() const {
    return on;
}