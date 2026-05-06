#pragma once

#include <memory>

#include "AbstractDustSensor.hpp"
#include "AbstractNetwork.hpp"

class DustSensor : public AbstractDustSensor {
   public:
    DustSensor() = default;
    explicit DustSensor(bool is_dust_detected);
    explicit DustSensor(std::shared_ptr<AbstractNetwork> network);

    bool isOn() const override;
    void turnOn() override;
    void turnOff() override;
    bool findDust() override;

   private:
    std::shared_ptr<AbstractNetwork> m_network = nullptr;
};
