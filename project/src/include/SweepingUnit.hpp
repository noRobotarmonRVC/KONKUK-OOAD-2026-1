#pragma once

#include <memory>

#include "AbstractCleaningUnit.hpp"
#include "AbstractNetwork.hpp"

class SweepingUnit : public AbstractCleaningUnit {
   public:
    SweepingUnit() = default;
    explicit SweepingUnit(std::shared_ptr<AbstractNetwork> network);

    bool isOn() const override;
    void turnOn() override;
    void turnOff() override;
    void clean(bool is_dust_detected) override;
    void boostMode() override;
    void normalMode() override;

   private:
    std::shared_ptr<AbstractNetwork> m_network = nullptr;
};
