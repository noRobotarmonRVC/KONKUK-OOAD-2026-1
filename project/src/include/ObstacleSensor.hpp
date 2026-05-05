#pragma once

#include <array>

#include "AbstractObstacleSensor.hpp"
#include "SimulatorClient.hpp"

class ObstacleSensor : public AbstractObstacleSensor {
   public:
    ObstacleSensor() = default;
    explicit ObstacleSensor(const std::array<int, 4>& obstacle_info);
    explicit ObstacleSensor(SimulatorClient& client);

    bool isOn() const override;
    void turnOn() override;
    void turnOff() override;
    std::array<int, 4> findObstacle() override;

   private:
    SimulatorClient* m_client = nullptr;
};
