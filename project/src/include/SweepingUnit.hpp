#pragma once

#include "AbstractCleaningUnit.hpp"
#include "SimulatorClient.hpp"

class SweepingUnit : public AbstractCleaningUnit {
   public:
    SweepingUnit() = default;
    explicit SweepingUnit(SimulatorClient& client);

    bool isOn() const override;
    void turnOn() override;
    void turnOff() override;
    void clean(bool is_dust_detected) override;
    void boostMode() override;
    void normalMode() override;

   private:
    SimulatorClient* m_client = nullptr;
};
