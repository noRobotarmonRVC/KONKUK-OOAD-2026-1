#pragma once

#include "CleaningController.hpp"

#include <memory>

class RVC {
private:
    bool on_ = false;
    std::unique_ptr<CleaningController> cleaningController_;

public:
    RVC();
    explicit RVC(std::unique_ptr<CleaningController> cleaningController);

    void powerOn();
    void powerOff();
    void startCleaning();
    void stopCleaning();
};
