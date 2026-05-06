#include <gtest/gtest.h>

#include <memory>
#include <stdexcept>

#include "RVC.hpp"

#ifdef USE_REAL_DEVICE

#include "CleaningController.hpp"
#include "DriveMotor.hpp"
#include "SweepingUnit.hpp"
#include "DustSensor.hpp"
#include "ObstacleSensor.hpp"

TEST(RVCTest, RealDevicePowerOnAndPowerOff) {
    auto motor = std::make_shared<DriveMotor>();
    auto cleaner = std::make_shared<SweepingUnit>();
    auto dustSensor = std::make_shared<DustSensor>();
    auto obstacleSensor = std::make_shared<ObstacleSensor>();

    auto cleaningController = std::make_shared<CleaningController>(
        motor,
        cleaner,
        dustSensor,
        obstacleSensor
    );

    RVC rvc(cleaningController);

    EXPECT_FALSE(rvc.isOn());

    rvc.powerOn();
    EXPECT_TRUE(rvc.isOn());

    rvc.powerOff();
    EXPECT_FALSE(rvc.isOn());
}

#else

#include <gmock/gmock.h>

#include "AbstractCleaningController.hpp"

using ::testing::InSequence;
using ::testing::StrictMock;

class MockCleaningController : public AbstractCleaningController {
public:
    MOCK_METHOD(void, run, (), (override));
    MOCK_METHOD(void, stop, (), (override));
    MOCK_METHOD(void, turnOnDeviceComponents, (), (override));
    MOCK_METHOD(void, turnOffDeviceComponents, (), (override));
    MOCK_METHOD(bool, isCleaning, (), (const, override));
};


TEST(RVCTest, DoNothing) {
    auto mock = std::make_shared<StrictMock<MockCleaningController>>();

    RVC rvc(mock);

    EXPECT_FALSE(rvc.isOn());
}

TEST(RVCTest, PowerOnOnly) {
    auto mock = std::make_shared<StrictMock<MockCleaningController>>();

    EXPECT_CALL(*mock, turnOnDeviceComponents()).Times(1);

    RVC rvc(mock);

    rvc.powerOn();

    EXPECT_TRUE(rvc.isOn());
}


TEST(RVCTest, PowerOffOnly) {
    auto mock = std::make_shared<StrictMock<MockCleaningController>>();

    RVC rvc(mock);

    rvc.powerOff();

    EXPECT_FALSE(rvc.isOn());
}


TEST(RVCTest, PowerOnThenPowerOff) {
    auto mock = std::make_shared<StrictMock<MockCleaningController>>();

    {
        InSequence seq;
        EXPECT_CALL(*mock, turnOnDeviceComponents()).Times(1);
        EXPECT_CALL(*mock, stop()).Times(1);
        EXPECT_CALL(*mock, turnOffDeviceComponents()).Times(1);
    }

    RVC rvc(mock);

    rvc.powerOn();
    rvc.powerOff();

    EXPECT_FALSE(rvc.isOn());
}


TEST(RVCTest, PowerOffThenPowerOn) {
    auto mock = std::make_shared<StrictMock<MockCleaningController>>();

    EXPECT_CALL(*mock, turnOnDeviceComponents()).Times(1);

    RVC rvc(mock);

    rvc.powerOff();
    rvc.powerOn();

    EXPECT_TRUE(rvc.isOn());
}


TEST(RVCTest, PowerOnThenPowerOffThenPowerOn) {
    auto mock = std::make_shared<StrictMock<MockCleaningController>>();

    {
        InSequence seq;
        EXPECT_CALL(*mock, turnOnDeviceComponents()).Times(1);
        EXPECT_CALL(*mock, stop()).Times(1);
        EXPECT_CALL(*mock, turnOffDeviceComponents()).Times(1);
        EXPECT_CALL(*mock, turnOnDeviceComponents()).Times(1);
    }

    RVC rvc(mock);

    rvc.powerOn();
    rvc.powerOff();
    rvc.powerOn();

    EXPECT_TRUE(rvc.isOn());
}


TEST(RVCTest, StartCleaningOnly) {
    auto mock = std::make_shared<StrictMock<MockCleaningController>>();

    RVC rvc(mock);

    rvc.startCleaning();

    EXPECT_FALSE(rvc.isOn());
}


TEST(RVCTest, PowerOnThenStartCleaning) {
    auto mock = std::make_shared<StrictMock<MockCleaningController>>();

    {
        InSequence seq;
        EXPECT_CALL(*mock, turnOnDeviceComponents()).Times(1);
        EXPECT_CALL(*mock, run()).Times(1);
    }

    RVC rvc(mock);

    rvc.powerOn();
    rvc.startCleaning();

    EXPECT_TRUE(rvc.isOn());
}


TEST(RVCTest, StartCleaningThenPowerOff) {
    auto mock = std::make_shared<StrictMock<MockCleaningController>>();

    RVC rvc(mock);

    rvc.startCleaning();
    rvc.powerOff();

    EXPECT_FALSE(rvc.isOn());
}

TEST(RVCTest, StopCleaningOnly) {
    auto mock = std::make_shared<StrictMock<MockCleaningController>>();

    EXPECT_CALL(*mock, stop()).Times(1);

    RVC rvc(mock);

    rvc.stopCleaning();

    EXPECT_FALSE(rvc.isOn());
}

TEST(RVCTest, StopCleaningThenPowerOn) {
    auto mock = std::make_shared<StrictMock<MockCleaningController>>();

    {
        InSequence seq;
        EXPECT_CALL(*mock, stop()).Times(1);
        EXPECT_CALL(*mock, turnOnDeviceComponents()).Times(1);
    }

    RVC rvc(mock);

    rvc.stopCleaning();
    rvc.powerOn();

    EXPECT_TRUE(rvc.isOn());
}

TEST(RVCTest, StartCleaningThenPowerOn) {
    auto mock = std::make_shared<StrictMock<MockCleaningController>>();

    EXPECT_CALL(*mock, turnOnDeviceComponents()).Times(1);

    RVC rvc(mock);

    rvc.startCleaning();
    rvc.powerOn();

    EXPECT_TRUE(rvc.isOn());
}

TEST(RVCTest, PowerOnThenStartCleaningThenStopCleaning) {
    auto mock = std::make_shared<StrictMock<MockCleaningController>>();

    {
        InSequence seq;
        EXPECT_CALL(*mock, turnOnDeviceComponents()).Times(1);
        EXPECT_CALL(*mock, run()).Times(1);
        EXPECT_CALL(*mock, stop()).Times(1);
    }

    RVC rvc(mock);

    rvc.powerOn();
    rvc.startCleaning();
    rvc.stopCleaning();

    EXPECT_TRUE(rvc.isOn());
}

TEST(RVCTest, PowerOnThenStartCleaningThenStopCleaningThenPowerOn) {
    auto mock = std::make_shared<StrictMock<MockCleaningController>>();

    {
        InSequence seq;
        EXPECT_CALL(*mock, turnOnDeviceComponents()).Times(1);
        EXPECT_CALL(*mock, run()).Times(1);
        EXPECT_CALL(*mock, stop()).Times(1);
    }

    RVC rvc(mock);

    rvc.powerOn();
    rvc.startCleaning();
    rvc.stopCleaning();
    rvc.powerOn();

    EXPECT_TRUE(rvc.isOn());
}

#endif