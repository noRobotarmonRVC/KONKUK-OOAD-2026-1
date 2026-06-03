#pragma once

#include <array>
#include <memory>

#include "AbstractNetwork.hpp"
#include "AbstractObstacleSensor.hpp"

class ObstacleSensor : public AbstractObstacleSensor {
   public:
    ObstacleSensor() = default;
    explicit ObstacleSensor(const std::array<int, 2>& obstacle_info);
    explicit ObstacleSensor(std::shared_ptr<AbstractNetwork> network);

    bool isOn() const override;
    void turnOn() override;
    void turnOff() override;
    std::array<int, 2> findObstacle() override;

   private:
    std::shared_ptr<AbstractNetwork> m_network = nullptr;
};
