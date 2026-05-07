<<<<<<< HEAD
#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <memory>
#include <string>

#include "AbstractNetwork.hpp"
#include "SweepingUnit.hpp"

using ::testing::InSequence;
using ::testing::Return;
using ::testing::StrictMock;
=======
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <memory>
#include <string>

#include "SweepingUnit.hpp"
#include "AbstractNetwork.hpp"

using ::testing::_;
>>>>>>> 7463e6af06c638a746fd9bdd0e161c535bc201f7

class MockNetwork : public AbstractNetwork {
public:
    MOCK_METHOD(void, connect, (), (override));
    MOCK_METHOD(void, send, (const std::string& cmd), (override));
    MOCK_METHOD(std::string, request, (const std::string& cmd), (override));
};

<<<<<<< HEAD
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
=======
class TestableSweepingUnit : public SweepingUnit {
public:
    using SweepingUnit::SweepingUnit;
    int getPower() const { return power; }
};

// ============================================================
// 전원 상태 테스트 (Network 불필요)
// ============================================================

TEST(SweepingUnitTest, isOn_Default_ReturnsFalse) {
    SweepingUnit unit;
    EXPECT_FALSE(unit.isOn());
}

TEST(SweepingUnitTest, turnOn_SetsIsOnTrue) {
    SweepingUnit unit;
    unit.turnOn();
    EXPECT_TRUE(unit.isOn());
}

TEST(SweepingUnitTest, turnOff_AfterTurnOn_ReturnsFalse) {
    SweepingUnit unit;
    unit.turnOn();
    unit.turnOff();
    EXPECT_FALSE(unit.isOn());
}

// ============================================================
// clean() — 전원 꺼진 상태: network send 호출 없어야 함
// ============================================================

TEST(SweepingUnitTest, clean_WhenOff_DustDetected_NoNetworkCall) {
    auto mock = std::make_shared<MockNetwork>();
    EXPECT_CALL(*mock, send(_)).Times(0);

    SweepingUnit unit(mock);
    unit.clean(true);
}

TEST(SweepingUnitTest, clean_WhenOff_NoDust_NoNetworkCall) {
    auto mock = std::make_shared<MockNetwork>();
    EXPECT_CALL(*mock, send(_)).Times(0);

    SweepingUnit unit(mock);
    unit.clean(false);
}

// ============================================================
// clean() — 전원 켜진 상태: 올바른 명령이 전송되어야 함
// ============================================================

TEST(SweepingUnitTest, clean_WhenOn_DustDetected_SendsBoostMode) {
    auto mock = std::make_shared<MockNetwork>();
    EXPECT_CALL(*mock, send("BOOST_MODE")).Times(1);

    SweepingUnit unit(mock);
>>>>>>> 7463e6af06c638a746fd9bdd0e161c535bc201f7
    unit.turnOn();
    unit.clean(true);
}

<<<<<<< HEAD
TEST(SweepingUnitTest, CleanWithoutDustSendsNormalMode) {
    auto network = std::make_shared<StrictMock<MockNetwork>>();

    EXPECT_CALL(*network, send("NORMAL_MODE")).Times(1);

    SweepingUnit unit(network);
=======
TEST(SweepingUnitTest, clean_WhenOn_NoDust_SendsNormalMode) {
    auto mock = std::make_shared<MockNetwork>();
    EXPECT_CALL(*mock, send("NORMAL_MODE")).Times(1);

    SweepingUnit unit(mock);
>>>>>>> 7463e6af06c638a746fd9bdd0e161c535bc201f7
    unit.turnOn();
    unit.clean(false);
}

<<<<<<< HEAD
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
=======
// ============================================================
// boostMode() / normalMode() 직접 호출
// ============================================================

TEST(SweepingUnitTest, boostMode_SendsBoostModeCommand) {
    auto mock = std::make_shared<MockNetwork>();
    EXPECT_CALL(*mock, send("BOOST_MODE")).Times(1);

    SweepingUnit unit(mock);
    unit.boostMode();
}

TEST(SweepingUnitTest, normalMode_SendsNormalModeCommand) {
    auto mock = std::make_shared<MockNetwork>();
    EXPECT_CALL(*mock, send("NORMAL_MODE")).Times(1);

    SweepingUnit unit(mock);
    unit.normalMode();
}

// ============================================================
// power 값 변경 테스트
// ============================================================

TEST(SweepingUnitTest, boostMode_SetsPowerTo2) {
    auto mock = std::make_shared<MockNetwork>();
    EXPECT_CALL(*mock, send("BOOST_MODE")).Times(1);

    TestableSweepingUnit unit(mock);
    unit.boostMode();
    EXPECT_EQ(unit.getPower(), 2);
}

TEST(SweepingUnitTest, normalMode_SetsPowerTo1) {
    auto mock = std::make_shared<MockNetwork>();
    EXPECT_CALL(*mock, send("NORMAL_MODE")).Times(1);

    TestableSweepingUnit unit(mock);
    unit.normalMode();
    EXPECT_EQ(unit.getPower(), 1);
>>>>>>> 7463e6af06c638a746fd9bdd0e161c535bc201f7
}
