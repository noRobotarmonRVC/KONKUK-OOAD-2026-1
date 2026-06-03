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

    std::array<int, 2> result = sensor.findObstacle();

    EXPECT_EQ(result, (std::array<int, 2>{0, 0}));
}

// network response rule: OBSTACLE front left right back
// return rule: [0] front, [1] left
// 1 = obstacle exists, 0 = open
class ObstacleSensorValidInputTest
    : public ::testing::TestWithParam<std::array<int, 2>> {
protected:
    static std::string makeResponse(const std::array<int, 2>& obstacleInfo) {
        const int front = obstacleInfo[0];
        const int left = obstacleInfo[1];

        const int right = 0;
        const int back = 0;

        std::ostringstream oss;
        oss << "OBSTACLE "
            << front << ' '
            << left << ' '
            << right << ' '
            << back;

        return oss.str();
    }
};

TEST_P(ObstacleSensorValidInputTest, ParsesValidObstacleResponseWhenPowerIsOn) {
    std::array<int, 2> expected = GetParam();

    auto network = std::make_shared<StrictMock<MockNetwork>>();

    EXPECT_CALL(*network, request("OBSTACLE_SENSOR_ON"))
        .Times(1)
        .WillOnce(Return("OK"));

    EXPECT_CALL(*network, request("FIND_OBSTACLE"))
        .Times(1)
        .WillOnce(Return(makeResponse(expected)));

    ObstacleSensor sensor(network);

    sensor.turnOn();

    std::array<int, 2> result = sensor.findObstacle();

    EXPECT_EQ(result, expected);
}

INSTANTIATE_TEST_SUITE_P(
    AllValidObstacleInputs,
    ObstacleSensorValidInputTest,
    ::testing::Values(
        std::array<int, 2>{0, 0},
        std::array<int, 2>{0, 1},
        std::array<int, 2>{1, 0},
        std::array<int, 2>{1, 1}
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

    EXPECT_EQ(sensor.findObstacle(), (std::array<int, 2>{0, 0}));
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

    EXPECT_EQ(sensor.findObstacle(), (std::array<int, 2>{0, 0}));
}

TEST(ObstacleSensorTest, FindObstacleRequestsNetworkEveryCall) {
    auto network = std::make_shared<StrictMock<MockNetwork>>();

    EXPECT_CALL(*network, request("OBSTACLE_SENSOR_ON"))
        .Times(1)
        .WillOnce(Return("OK"));

    EXPECT_CALL(*network, request("FIND_OBSTACLE"))
        .Times(2)
        .WillOnce(Return("OBSTACLE 1 0 0 0"))  // 앞=1, 왼쪽=0
        .WillOnce(Return("OBSTACLE 0 1 0 0")); // 앞=0, 왼쪽=1

    ObstacleSensor sensor(network);

    sensor.turnOn();

    EXPECT_EQ(sensor.findObstacle(), (std::array<int, 2>{1, 0}));
    EXPECT_EQ(sensor.findObstacle(), (std::array<int, 2>{0, 1}));
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

    EXPECT_EQ(sensor.findObstacle(), (std::array<int, 2>{0, 0}));
}