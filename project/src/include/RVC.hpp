#pragma once

#include <memory>

#include "AbstractCleaningController.hpp"

class RVC {
private:
    bool on = false;
    std::shared_ptr<AbstractCleaningController> cleaningController;

public:
    explicit RVC(std::shared_ptr<AbstractCleaningController> cleaningController);

    void powerOn();
    void powerOff();

    void startCleaning();
    void stopCleaning();

    bool isOn() const;
};