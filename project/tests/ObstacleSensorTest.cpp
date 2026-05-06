#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <array>
#include <memory>
#include <sstream>
#include <string>

#include "AbstractNetwork.hpp"
#include "ObstacleSensor.hpp"

using ::testing::Return;
using ::testing::StrictMock;

class MockNetwork : public AbstractNetwork {
public:
    MOCK_METHOD(void, connect, (), (override));
    MOCK_METHOD(void, send, (const std::string& cmd), (override));
    MOCK_METHOD(std::string, request, (const std::string& cmd), (override));
};

TEST(ObstacleSensorTest, InitialStateIsOff) {
    auto network = std::make_shared<StrictMock<MockNetwork>>();

    ObstacleSensor sensor(network);

    EXPECT_FALSE(sensor.isOn());
}

TEST(ObstacleSensorTest, TurnOnMakesSensorOn) {
    auto network = std::make_shared<StrictMock<MockNetwork>>();

    ObstacleSensor sensor(network);

    sensor.turnOn();

    EXPECT_TRUE(sensor.isOn());
}

TEST(ObstacleSensorTest, TurnOffMakesSensorOff) {
    auto network = std::make_shared<StrictMock<MockNetwork>>();

    ObstacleSensor sensor(network);

    sensor.turnOn();
    sensor.turnOff();

    EXPECT_FALSE(sensor.isOn());
}

TEST(ObstacleSensorTest, FindObstacleReturnsZerosWhenPowerIsOff) {
    auto network = std::make_shared<StrictMock<MockNetwork>>();

    ObstacleSensor sensor(network);

    std::array<int, 4> result = sensor.findObstacle();

    EXPECT_EQ(result, (std::array<int, 4>{0, 0, 0, 0}));
}

class ObstacleSensorValidInputTest
    : public ::testing::TestWithParam<std::array<int, 4>> {
protected:
    static std::string makeResponse(const std::array<int, 4>& obstacleInfo) {
        std::ostringstream oss;
        oss << "OBSTACLE "
            << obstacleInfo[0] << ' '
            << obstacleInfo[1] << ' '
            << obstacleInfo[2] << ' '
            << obstacleInfo[3];

        return oss.str();
    }
};

TEST_P(ObstacleSensorValidInputTest, ParsesValidObstacleResponseWhenPowerIsOn) {
    std::array<int, 4> expected = GetParam();

    auto network = std::make_shared<StrictMock<MockNetwork>>();

    EXPECT_CALL(*network, request("FIND_OBSTACLE"))
        .Times(1)
        .WillOnce(Return(makeResponse(expected)));

    ObstacleSensor sensor(network);

    sensor.turnOn();

    std::array<int, 4> result = sensor.findObstacle();

    EXPECT_EQ(result, expected);
}

INSTANTIATE_TEST_SUITE_P(
    AllValidObstacleInputs,
    ObstacleSensorValidInputTest,
    ::testing::Values(
        std::array<int, 4>{0, 0, 0, 0},
        std::array<int, 4>{0, 0, 0, 1},
        std::array<int, 4>{0, 0, 1, 0},
        std::array<int, 4>{0, 0, 1, 1},
        std::array<int, 4>{0, 1, 0, 0},
        std::array<int, 4>{0, 1, 0, 1},
        std::array<int, 4>{0, 1, 1, 0},
        std::array<int, 4>{0, 1, 1, 1},
        std::array<int, 4>{1, 0, 0, 0},
        std::array<int, 4>{1, 0, 0, 1},
        std::array<int, 4>{1, 0, 1, 0},
        std::array<int, 4>{1, 0, 1, 1},
        std::array<int, 4>{1, 1, 0, 0},
        std::array<int, 4>{1, 1, 0, 1},
        std::array<int, 4>{1, 1, 1, 0},
        std::array<int, 4>{1, 1, 1, 1}
    )
);