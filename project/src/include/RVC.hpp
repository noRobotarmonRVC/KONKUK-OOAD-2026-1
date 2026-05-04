#pragma once

#include <memory>

#include "CleaningController.hpp"

class RVC {
   private:
    bool on = false;
    std::shared_ptr<CleaningController> cleaning_controller;

   public:
    RVC();
    explicit RVC(std::shared_ptr<CleaningController> cleaning_controller);

    void powerOn();
    void powerOff();
    void startCleaning();
    void stopCleaning();
};
