#include <gtest/gtest.h>

#include <array>
#include <chrono>
#include <memory>
#include <stdexcept>
#include <thread>

#include "CleaningController.hpp"

#if USE_REAL_DEVICE

#include "DriveMotor.hpp"
#include "SweepingUnit.hpp"
#include "DustSensor.hpp"
#include "ObstacleSensor.hpp"

#include "DriveController.hpp"
#include "SweepingController.hpp"
#include "DevicePowerManager.hpp"
#include "DeviceComponent.hpp"
#include "TCPNetwork.hpp"

namespace {

std::unique_ptr<CleaningController> makeRealCleaningController() {
    auto network = std::make_shared<TCPNetwork>("127.0.0.1", 9000);

    auto motor = std::make_shared<DriveMotor>(network);
    auto cleaner = std::make_shared<SweepingUnit>(network);
    auto dustSensor = std::make_shared<DustSensor>(network);
    auto obstacleSensor = std::make_shared<ObstacleSensor>(network);

    auto driveController = std::make_shared<DriveController>(motor);
    auto sweepingController = std::make_shared<SweepingController>(cleaner);

    auto powerManager = std::make_shared<DevicePowerManager>(
        std::array<std::shared_ptr<DeviceComponent>, 3>{
            cleaner,
            dustSensor,
            obstacleSensor
        }
    );

    return std::make_unique<CleaningController>(
        driveController,
        sweepingController,
        powerManager,
        dustSensor,
        obstacleSensor
    );
}

}  // namespace

TEST(CleaningControllerTest, RealDeviceInitialStateIsNotCleaning) {
    auto controller = makeRealCleaningController();

    EXPECT_FALSE(controller->isCleaning());
}

TEST(CleaningControllerTest, RealDeviceTurnOnAndTurnOffComponents) {
    auto controller = makeRealCleaningController();

    controller->turnOnDeviceComponents();
    controller->turnOffDeviceComponents();

    EXPECT_FALSE(controller->isCleaning());
}

TEST(CleaningControllerTest, RealDeviceStopWhenNotCleaningDoesNothing) {
    auto controller = makeRealCleaningController();

    controller->stop();

    EXPECT_FALSE(controller->isCleaning());
}

#else

#include <gmock/gmock.h>

#include "AbstractDevicePowerManager.hpp"
#include "AbstractDriveController.hpp"
#include "AbstractDustSensor.hpp"
#include "AbstractObstacleSensor.hpp"
#include "AbstractSweepingController.hpp"

using ::testing::AnyNumber;
using ::testing::InSequence;
using ::testing::Return;
using ::testing::StrictMock;

class MockDriveController : public AbstractDriveController {
public:
    MOCK_METHOD(bool, avoid, ((const std::array<int, 2>&), int), (override));
    MOCK_METHOD(void, moveForward, (), (override));
    MOCK_METHOD(void, stop, (), (override));
};

class MockSweepingController : public AbstractSweepingController {
public:
    MOCK_METHOD(void, turnOn, (), (override));
    MOCK_METHOD(void, turnOff, (), (override));
    MOCK_METHOD(bool, isOn, (), (const, override));
    MOCK_METHOD(void, clean, (bool isDustDetected), (override));
};

class MockDevicePowerManager : public AbstractDevicePowerManager {
public:
    MOCK_METHOD(void, allTurnOn, (), (override));
    MOCK_METHOD(void, allTurnOff, (), (override));
};

class MockDustSensor : public AbstractDustSensor {
public:
    MOCK_METHOD(bool, findDust, (), (override));
    MOCK_METHOD(bool, isOn, (), (const, override));
    MOCK_METHOD(void, turnOn, (), (override));
    MOCK_METHOD(void, turnOff, (), (override));
};

class MockObstacleSensor : public AbstractObstacleSensor {
public:
    MOCK_METHOD((std::array<int, 2>), findObstacle, (), (override));
    MOCK_METHOD(bool, isOn, (), (const, override));
    MOCK_METHOD(void, turnOn, (), (override));
    MOCK_METHOD(void, turnOff, (), (override));
};

class CleaningControllerMockTest : public ::testing::Test {
protected:
    std::shared_ptr<StrictMock<MockDriveController>> drive =
        std::make_shared<StrictMock<MockDriveController>>();

    std::shared_ptr<StrictMock<MockSweepingController>> sweeping =
        std::make_shared<StrictMock<MockSweepingController>>();

    std::shared_ptr<StrictMock<MockDevicePowerManager>> powerManager =
        std::make_shared<StrictMock<MockDevicePowerManager>>();

    std::shared_ptr<StrictMock<MockDustSensor>> dustSensor =
        std::make_shared<StrictMock<MockDustSensor>>();

    std::shared_ptr<StrictMock<MockObstacleSensor>> obstacleSensor =
        std::make_shared<StrictMock<MockObstacleSensor>>();

    std::unique_ptr<CleaningController> makeController() {
        return std::make_unique<CleaningController>(
            drive,
            sweeping,
            powerManager,
            dustSensor,
            obstacleSensor
        );
    }

    void allowCleaningLoopIdle() {
        EXPECT_CALL(*obstacleSensor, findObstacle())
            .Times(AnyNumber())
            .WillRepeatedly(Return(std::array<int, 2>{0, 0}));

        EXPECT_CALL(*dustSensor, findDust())
            .Times(AnyNumber())
            .WillRepeatedly(Return(false));

        EXPECT_CALL(*sweeping, clean(false))
            .Times(AnyNumber());

        EXPECT_CALL(*sweeping, isOn())
            .Times(AnyNumber())
            .WillRepeatedly(Return(false));
    }

    static void stopAndWaitDetachedThread(CleaningController& controller) {
        controller.stop();

        std::this_thread::sleep_for(std::chrono::milliseconds(1200));

        EXPECT_FALSE(controller.isCleaning());
    }
};

TEST_F(CleaningControllerMockTest, ConstructorThrowsWhenDriveControllerIsNull) {
    EXPECT_THROW(
        CleaningController(
            nullptr,
            sweeping,
            powerManager,
            dustSensor,
            obstacleSensor
        ),
        std::invalid_argument
    );
}

TEST_F(CleaningControllerMockTest, ConstructorThrowsWhenSweepingControllerIsNull) {
    EXPECT_THROW(
        CleaningController(
            drive,
            nullptr,
            powerManager,
            dustSensor,
            obstacleSensor
        ),
        std::invalid_argument
    );
}

TEST_F(CleaningControllerMockTest, ConstructorThrowsWhenDustSensorIsNull) {
    EXPECT_THROW(
        CleaningController(
            drive,
            sweeping,
            powerManager,
            nullptr,
            obstacleSensor
        ),
        std::invalid_argument
    );
}

TEST_F(CleaningControllerMockTest, ConstructorThrowsWhenObstacleSensorIsNull) {
    EXPECT_THROW(
        CleaningController(
            drive,
            sweeping,
            powerManager,
            dustSensor,
            nullptr
        ),
        std::invalid_argument
    );
}

TEST_F(CleaningControllerMockTest, RunOnly) {
    auto controller = makeController();

    allowCleaningLoopIdle();

    controller->run();

    EXPECT_TRUE(controller->isCleaning());

    stopAndWaitDetachedThread(*controller);
}

TEST_F(CleaningControllerMockTest, StopOnly) {
    auto controller = makeController();

    controller->stop();

    EXPECT_FALSE(controller->isCleaning());
}

TEST_F(CleaningControllerMockTest, TurnOnThenRun) {
    auto controller = makeController();

    EXPECT_CALL(*powerManager, allTurnOn()).Times(1);

    allowCleaningLoopIdle();

    controller->turnOnDeviceComponents();
    controller->run();

    EXPECT_TRUE(controller->isCleaning());

    stopAndWaitDetachedThread(*controller);
}

TEST_F(CleaningControllerMockTest, TurnOnThenRunThenStop) {
    auto controller = makeController();

    EXPECT_CALL(*powerManager, allTurnOn()).Times(1);

    allowCleaningLoopIdle();

    controller->turnOnDeviceComponents();
    controller->run();

    EXPECT_TRUE(controller->isCleaning());

    stopAndWaitDetachedThread(*controller);
}

TEST_F(CleaningControllerMockTest, TurnOnThenRunThenTurnOff) {
    auto controller = makeController();

    EXPECT_CALL(*powerManager, allTurnOn()).Times(1);
    EXPECT_CALL(*powerManager, allTurnOff()).Times(1);

    allowCleaningLoopIdle();

    controller->turnOnDeviceComponents();
    controller->run();

    EXPECT_TRUE(controller->isCleaning());

    controller->turnOffDeviceComponents();

    EXPECT_TRUE(controller->isCleaning());

    stopAndWaitDetachedThread(*controller);
}

TEST_F(CleaningControllerMockTest, RunThenStop) {
    auto controller = makeController();

    allowCleaningLoopIdle();

    controller->run();

    EXPECT_TRUE(controller->isCleaning());

    stopAndWaitDetachedThread(*controller);
}

TEST_F(CleaningControllerMockTest, TurnOnThenRunThenStopAgain) {
    auto controller = makeController();

    EXPECT_CALL(*powerManager, allTurnOn()).Times(1);

    allowCleaningLoopIdle();

    controller->turnOnDeviceComponents();
    controller->run();

    EXPECT_TRUE(controller->isCleaning());

    stopAndWaitDetachedThread(*controller);
}

TEST_F(CleaningControllerMockTest, TurnOnThenTurnOff) {
    auto controller = makeController();

    {
        InSequence seq;

        EXPECT_CALL(*powerManager, allTurnOn()).Times(1);
        EXPECT_CALL(*powerManager, allTurnOff()).Times(1);
    }

    controller->turnOnDeviceComponents();
    controller->turnOffDeviceComponents();

    EXPECT_FALSE(controller->isCleaning());
}

TEST_F(CleaningControllerMockTest, TurnOffThenTurnOn) {
    auto controller = makeController();

    {
        InSequence seq;

        EXPECT_CALL(*powerManager, allTurnOff()).Times(1);
        EXPECT_CALL(*powerManager, allTurnOn()).Times(1);
    }

    controller->turnOffDeviceComponents();
    controller->turnOnDeviceComponents();

    EXPECT_FALSE(controller->isCleaning());
}

TEST_F(CleaningControllerMockTest, TurnOnThreeTimes) {
    auto controller = makeController();

    EXPECT_CALL(*powerManager, allTurnOn()).Times(3);

    controller->turnOnDeviceComponents();
    controller->turnOnDeviceComponents();
    controller->turnOnDeviceComponents();

    EXPECT_FALSE(controller->isCleaning());
}

#endif