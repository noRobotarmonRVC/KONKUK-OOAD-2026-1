#pragma once

#include "AbstractDustSensor.hpp"
#include "SimulatorClient.hpp"

class DustSensor : public AbstractDustSensor {
   public:
    DustSensor() = default;
    explicit DustSensor(bool is_dust_detected);
    explicit DustSensor(SimulatorClient& client);

    bool isOn() const override;
    void turnOn() override;
    void turnOff() override;
    bool findDust() override;

   private:
    SimulatorClient* m_client = nullptr;
};
