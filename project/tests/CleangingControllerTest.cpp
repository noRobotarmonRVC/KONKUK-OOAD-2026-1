#include <gtest/gtest.h>

#include <array>
#include <chrono>
#include <future>
#include <memory>
#include <stdexcept>
#include <thread>

#include "CleaningController.hpp"

#ifdef USE_REAL_DEVICE

#include "DriveMotor.hpp"
#include "SweepingUnit.hpp"
#include "DustSensor.hpp"
#include "ObstacleSensor.hpp"

TEST(CleaningControllerTest, RealDeviceInitialStateIsNotCleaning) {
    auto motor = std::make_shared<DriveMotor>();
    auto cleaner = std::make_shared<SweepingUnit>();
    auto dustSensor = std::make_shared<DustSensor>();
    auto obstacleSensor = std::make_shared<ObstacleSensor>();

    CleaningController controller(
        motor,
        cleaner,
        dustSensor,
        obstacleSensor
    );

    EXPECT_FALSE(controller.isCleaning());
}

TEST(CleaningControllerTest, RealDeviceTurnOnAndTurnOffComponents) {
    auto motor = std::make_shared<DriveMotor>();
    auto cleaner = std::make_shared<SweepingUnit>();
    auto dustSensor = std::make_shared<DustSensor>();
    auto obstacleSensor = std::make_shared<ObstacleSensor>();

    CleaningController controller(
        motor,
        cleaner,
        dustSensor,
        obstacleSensor
    );

    controller.turnOnDeviceComponents();
    controller.turnOffDeviceComponents();

    EXPECT_FALSE(controller.isCleaning());
}

TEST(CleaningControllerTest, RealDeviceStopWhenNotCleaningDoesNothing) {
    auto motor = std::make_shared<DriveMotor>();
    auto cleaner = std::make_shared<SweepingUnit>();
    auto dustSensor = std::make_shared<DustSensor>();
    auto obstacleSensor = std::make_shared<ObstacleSensor>();

    CleaningController controller(
        motor,
        cleaner,
        dustSensor,
        obstacleSensor
    );

    controller.stop();

    EXPECT_FALSE(controller.isCleaning());
}

#else

#include <gmock/gmock.h>

#include "AbstractDevicePowerManager.hpp"
#include "AbstractDriveController.hpp"
#include "AbstractDustSensor.hpp"
#include "AbstractObstacleSensor.hpp"
#include "AbstractSweepingController.hpp"

using ::testing::InSequence;
using ::testing::Return;
using ::testing::StrictMock;

class MockDriveController : public AbstractDriveController {
public:
    MOCK_METHOD(void, avoid, ((const std::array<int, 4>&)), (override));
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
    MOCK_METHOD((std::array<int, 4>), findObstacle, (), (override));

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

    static bool waitUntilNotCleaning(
        CleaningController& controller,
        std::chrono::milliseconds timeout = std::chrono::milliseconds(700)
    ) {
        const auto deadline = std::chrono::steady_clock::now() + timeout;

        while (std::chrono::steady_clock::now() < deadline) {
            if (!controller.isCleaning()) {
                std::this_thread::sleep_for(std::chrono::milliseconds(10));
                return true;
            }

            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }

        return !controller.isCleaning();
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

    controller->run();

    EXPECT_FALSE(controller->isCleaning());
}
// run함수 잘못만든 거 확인할 수 있었음 !안 붙여서 꺼저도 run가능했었음

TEST_F(CleaningControllerMockTest, StopOnly) {
    auto controller = makeController();

    controller->stop();

    EXPECT_FALSE(controller->isCleaning());
}
// 꺼져있음에도 호출되는거 확인

TEST_F(CleaningControllerMockTest, TurnOnThenRun) {
    auto controller = makeController();

    EXPECT_CALL(*powerManager, allTurnOn()).Times(1);
    controller->turnOnDeviceComponents();
    controller->run();

    ASSERT_TRUE(waitUntilNotCleaning(*controller));
    EXPECT_FALSE(controller->isCleaning());
}

TEST_F(CleaningControllerMockTest, TurnOnThenRunThenStop) {
    auto controller = makeController();

    EXPECT_CALL(*powerManager, allTurnOn()).Times(1);

    controller->turnOnDeviceComponents();
    controller->run();


    controller->stop();
    
    ASSERT_TRUE(waitUntilNotCleaning(*controller));
    EXPECT_FALSE(controller->isCleaning());
}

TEST_F(CleaningControllerMockTest, TurnOnThenRunThenTurnOff) {
    auto controller = makeController();

    EXPECT_CALL(*powerManager, allTurnOn()).Times(1);

    controller->turnOnDeviceComponents();
    controller->run();

    ASSERT_TRUE(waitUntilNotCleaning(*controller));

    EXPECT_CALL(*powerManager, allTurnOff()).Times(1);

    controller->turnOffDeviceComponents();

    EXPECT_FALSE(controller->isCleaning());
}

TEST_F(CleaningControllerMockTest, RunThenStop) {
    auto controller = makeController();

    controller->run();

    ASSERT_TRUE(waitUntilNotCleaning(*controller));

    controller->stop();

    EXPECT_FALSE(controller->isCleaning());
}

TEST_F(CleaningControllerMockTest, TurnOnThenRunThenStopAgain) {
    auto controller = makeController();

    EXPECT_CALL(*powerManager, allTurnOn()).Times(1);

    controller->turnOnDeviceComponents();
    controller->run();

    ASSERT_TRUE(waitUntilNotCleaning(*controller));

    controller->stop();

    EXPECT_FALSE(controller->isCleaning());
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