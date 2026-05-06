#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <memory>
#include <string>

#include "AbstractNetwork.hpp"
#include "DustSensor.hpp"

using ::testing::Return;
using ::testing::StrictMock;

class MockNetwork : public AbstractNetwork {
public:
    MOCK_METHOD(void, connect, (), (override));
    MOCK_METHOD(void, send, (const std::string& cmd), (override));
    MOCK_METHOD(std::string, request, (const std::string& cmd), (override));
};

TEST(DustSensorTest, InitialStateIsOff) {
    auto network = std::make_shared<StrictMock<MockNetwork>>();

    DustSensor sensor(network);

    EXPECT_FALSE(sensor.isOn());
}

TEST(DustSensorTest, TurnOnMakesSensorOn) {
    auto network = std::make_shared<StrictMock<MockNetwork>>();

    DustSensor sensor(network);

    sensor.turnOn();

    EXPECT_TRUE(sensor.isOn());
}

TEST(DustSensorTest, TurnOffMakesSensorOff) {
    auto network = std::make_shared<StrictMock<MockNetwork>>();

    DustSensor sensor(network);

    sensor.turnOn();
    sensor.turnOff();

    EXPECT_FALSE(sensor.isOn());
}

TEST(DustSensorTest, FindDustReturnsFalseWhenPowerIsOff) {
    auto network = std::make_shared<StrictMock<MockNetwork>>();

    DustSensor sensor(network);

    bool result = sensor.findDust();

    EXPECT_FALSE(result);
}

TEST(DustSensorTest, FindDustRequestsNetworkWhenPowerIsOn) {
    auto network = std::make_shared<StrictMock<MockNetwork>>();

    EXPECT_CALL(*network, request("FIND_DUST"))
        .Times(1)
        .WillOnce(Return("DUST 1"));

    DustSensor sensor(network);

    sensor.turnOn();

    bool result = sensor.findDust();

    EXPECT_TRUE(result);
}

TEST(DustSensorTest, FindDustReturnsTrueWhenNetworkReturnsDustOne) {
    auto network = std::make_shared<StrictMock<MockNetwork>>();

    EXPECT_CALL(*network, request("FIND_DUST"))
        .Times(1)
        .WillOnce(Return("DUST 1"));

    DustSensor sensor(network);

    sensor.turnOn();

    EXPECT_TRUE(sensor.findDust());
}

TEST(DustSensorTest, FindDustReturnsFalseWhenNetworkReturnsDustZero) {
    auto network = std::make_shared<StrictMock<MockNetwork>>();

    EXPECT_CALL(*network, request("FIND_DUST"))
        .Times(1)
        .WillOnce(Return("DUST 0"));

    DustSensor sensor(network);

    sensor.turnOn();

    EXPECT_FALSE(sensor.findDust());
}

TEST(DustSensorTest, FindDustReturnsFalseForUnexpectedResponse) {
    auto network = std::make_shared<StrictMock<MockNetwork>>();

    EXPECT_CALL(*network, request("FIND_DUST"))
        .Times(1)
        .WillOnce(Return("INVALID"));

    DustSensor sensor(network);

    sensor.turnOn();

    EXPECT_FALSE(sensor.findDust());
}

TEST(DustSensorTest, FindDustRequestsNetworkEveryTimeWhenPowerIsOn) {
    auto network = std::make_shared<StrictMock<MockNetwork>>();

    EXPECT_CALL(*network, request("FIND_DUST"))
        .Times(2)
        .WillOnce(Return("DUST 1"))
        .WillOnce(Return("DUST 0"));

    DustSensor sensor(network);

    sensor.turnOn();

    EXPECT_TRUE(sensor.findDust());
    EXPECT_FALSE(sensor.findDust());
}

TEST(DustSensorTest, TurnOffPreventsNetworkRequestAfterTurnOn) {
    auto network = std::make_shared<StrictMock<MockNetwork>>();

    DustSensor sensor(network);

    sensor.turnOn();
    sensor.turnOff();

    bool result = sensor.findDust();

    EXPECT_FALSE(result);
}

TEST(DustSensorTest, InitialDustConstructorIsOffByDefault) {
    DustSensor sensor(true);

    EXPECT_FALSE(sensor.isOn());
}

TEST(DustSensorTest, InitialDustConstructorFindDustReturnsFalseWhenOff) {
    DustSensor sensor(true);

    EXPECT_FALSE(sensor.findDust());
}