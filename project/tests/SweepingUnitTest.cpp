#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <memory>
#include <string>

#include "AbstractNetwork.hpp"
#include "SweepingUnit.hpp"

using ::testing::InSequence;
using ::testing::Return;
using ::testing::StrictMock;

class MockNetwork : public AbstractNetwork {
   public:
    MOCK_METHOD(void, connect, (), (override));
    MOCK_METHOD(void, send, (const std::string& cmd), (override));
    MOCK_METHOD(std::string, request, (const std::string& cmd), (override));
};

TEST(SweepingUnitTest, InitialStateIsOff) {
    auto network = std::make_shared<StrictMock<MockNetwork>>();

    SweepingUnit unit(network);

    EXPECT_FALSE(unit.isOn());
}

TEST(SweepingUnitTest, TurnOnMakesUnitOn) {
    auto network = std::make_shared<StrictMock<MockNetwork>>();

    SweepingUnit unit(network);
    unit.turnOn();

    EXPECT_TRUE(unit.isOn());
}

TEST(SweepingUnitTest, TurnOffMakesUnitOff) {
    auto network = std::make_shared<StrictMock<MockNetwork>>();

    SweepingUnit unit(network);
    unit.turnOn();
    unit.turnOff();

    EXPECT_FALSE(unit.isOn());
}

TEST(SweepingUnitTest, CleanDoesNothingWhenOff) {
    auto network = std::make_shared<StrictMock<MockNetwork>>();

    SweepingUnit unit(network);

    // StrictMock이므로 send()가 호출되면 테스트 실패
    unit.clean(true);
    unit.clean(false);
}

TEST(SweepingUnitTest, CleanWithDustSendsBoostMode) {
    auto network = std::make_shared<StrictMock<MockNetwork>>();

    EXPECT_CALL(*network, send("BOOST_MODE")).Times(1);

    SweepingUnit unit(network);
    unit.turnOn();
    unit.clean(true);
}

TEST(SweepingUnitTest, CleanWithoutDustSendsNormalMode) {
    auto network = std::make_shared<StrictMock<MockNetwork>>();

    EXPECT_CALL(*network, send("NORMAL_MODE")).Times(1);

    SweepingUnit unit(network);
    unit.turnOn();
    unit.clean(false);
}

TEST(SweepingUnitTest, BoostModeDirectlySendsBoostMode) {
    auto network = std::make_shared<StrictMock<MockNetwork>>();

    EXPECT_CALL(*network, send("BOOST_MODE")).Times(1);

    SweepingUnit unit(network);
    unit.boostMode();
}

TEST(SweepingUnitTest, NormalModeDirectlySendsNormalMode) {
    auto network = std::make_shared<StrictMock<MockNetwork>>();

    EXPECT_CALL(*network, send("NORMAL_MODE")).Times(1);

    SweepingUnit unit(network);
    unit.normalMode();
}

TEST(SweepingUnitTest, CleanSwitchesFromBoostToNormal) {
    auto network = std::make_shared<StrictMock<MockNetwork>>();

    {
        InSequence seq;
        EXPECT_CALL(*network, send("BOOST_MODE")).Times(1);
        EXPECT_CALL(*network, send("NORMAL_MODE")).Times(1);
    }

    SweepingUnit unit(network);
    unit.turnOn();
    unit.clean(true);
    unit.clean(false);
}

TEST(SweepingUnitTest, TurnOffPreventsCleanFromSendingCommands) {
    auto network = std::make_shared<StrictMock<MockNetwork>>();

    SweepingUnit unit(network);
    unit.turnOn();
    unit.turnOff();

    // StrictMock이므로 send()가 호출되면 테스트 실패
    unit.clean(true);
    unit.clean(false);
}
