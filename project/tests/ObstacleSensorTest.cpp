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
    MOCK_METHOD(std::string, request, (const std::string& cmd), (override));
};

TEST(ObstacleSensorTest, InitialStateIsOff) {
    auto network = std::make_shared<StrictMock<MockNetwork>>();

    ObstacleSensor sensor(network);

    EXPECT_FALSE(sensor.isOn());
}

TEST(ObstacleSensorTest, TurnOnMakesSensorOn) {
    auto network = std::make_shared<StrictMock<MockNetwork>>();

    EXPECT_CALL(*network, request("OBSTACLE_SENSOR_ON"))
        .Times(1)
        .WillOnce(Return("OK"));

    ObstacleSensor sensor(network);

    sensor.turnOn();

    EXPECT_TRUE(sensor.isOn());
}

TEST(ObstacleSensorTest, TurnOffMakesSensorOff) {
    auto network = std::make_shared<StrictMock<MockNetwork>>();

    EXPECT_CALL(*network, request("OBSTACLE_SENSOR_ON"))
        .Times(1)
        .WillOnce(Return("OK"));

    EXPECT_CALL(*network, request("OBSTACLE_SENSOR_OFF"))
        .Times(1)
        .WillOnce(Return("OK"));

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

// index rule: [0] front, [1] right, [2] back, [3] left
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

    EXPECT_CALL(*network, request("OBSTACLE_SENSOR_ON"))
        .Times(1)
        .WillOnce(Return("OK"));

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

TEST(ObstacleSensorTest, FindObstacleReturnsZerosOnInvalidResponse) {
    auto network = std::make_shared<StrictMock<MockNetwork>>();

    EXPECT_CALL(*network, request("OBSTACLE_SENSOR_ON"))
        .Times(1)
        .WillOnce(Return("OK"));

    EXPECT_CALL(*network, request("FIND_OBSTACLE"))
        .Times(1)
        .WillOnce(Return("GARBAGE"));

    ObstacleSensor sensor(network);

    sensor.turnOn();

    EXPECT_EQ(sensor.findObstacle(), (std::array<int, 4>{0, 0, 0, 0}));
}

TEST(ObstacleSensorTest, FindObstacleReturnsZerosOnEmptyResponse) {
    auto network = std::make_shared<StrictMock<MockNetwork>>();

    EXPECT_CALL(*network, request("OBSTACLE_SENSOR_ON"))
        .Times(1)
        .WillOnce(Return("OK"));

    EXPECT_CALL(*network, request("FIND_OBSTACLE"))
        .Times(1)
        .WillOnce(Return(""));

    ObstacleSensor sensor(network);

    sensor.turnOn();

    EXPECT_EQ(sensor.findObstacle(), (std::array<int, 4>{0, 0, 0, 0}));
}

TEST(ObstacleSensorTest, FindObstacleRequestsNetworkEveryCall) {
    auto network = std::make_shared<StrictMock<MockNetwork>>();

    EXPECT_CALL(*network, request("OBSTACLE_SENSOR_ON"))
        .Times(1)
        .WillOnce(Return("OK"));

    EXPECT_CALL(*network, request("FIND_OBSTACLE"))
        .Times(2)
        .WillOnce(Return("OBSTACLE 1 0 0 0"))
        .WillOnce(Return("OBSTACLE 0 0 0 0"));

    ObstacleSensor sensor(network);

    sensor.turnOn();

    EXPECT_EQ(sensor.findObstacle(), (std::array<int, 4>{1, 0, 0, 0}));
    EXPECT_EQ(sensor.findObstacle(), (std::array<int, 4>{0, 0, 0, 0}));
}

TEST(ObstacleSensorTest, TurnOffPreventsObstacleSensorFromQuerying) {
    auto network = std::make_shared<StrictMock<MockNetwork>>();

    EXPECT_CALL(*network, request("OBSTACLE_SENSOR_ON"))
        .Times(1)
        .WillOnce(Return("OK"));

    EXPECT_CALL(*network, request("OBSTACLE_SENSOR_OFF"))
        .Times(1)
        .WillOnce(Return("OK"));

    ObstacleSensor sensor(network);

    sensor.turnOn();
    sensor.turnOff();

    // StrictMock이므로 FIND_OBSTACLE이 호출되면 테스트 실패
    EXPECT_EQ(sensor.findObstacle(), (std::array<int, 4>{0, 0, 0, 0}));
}