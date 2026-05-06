#include <gtest/gtest.h>

#include <memory>
#include <stdexcept>

#include "RVC.hpp"
#include "CleaningController.hpp"

#include "AbstractDriveMotor.hpp"
#include "SweepingUnit.hpp"
#include "DustSensor.hpp"
#include "ObstacleSensor.hpp"

// ============================================================
// MockDriveMotor
// gmock 없이 순수 gtest만으로 만든 수동 Mock
// ============================================================

class MockDriveMotor : public AbstractDriveMotor {
public:
    int moveForwardCallCount = 0;
    int moveBackwardCallCount = 0;
    int stopCallCount = 0;
    int rotateRightCallCount = 0;
    int rotateLeftCallCount = 0;

    void moveForward() override {
        moveForwardCallCount++;
    }

    void moveBackward() override {
        moveBackwardCallCount++;
    }

    void stop() override {
        stopCallCount++;
    }

    void rotateRight() override {
        rotateRightCallCount++;
    }

    void rotateLeft() override {
        rotateLeftCallCount++;
    }

    int totalCallCount() const {
        return moveForwardCallCount
             + moveBackwardCallCount
             + stopCallCount
             + rotateRightCallCount
             + rotateLeftCallCount;
    }
};

// ============================================================
// 테스트용 RVC 생성 헬퍼
// real CleaningController + mock motor 조합
// ============================================================

struct RVCTestFixture {
    std::shared_ptr<MockDriveMotor> mockMotor;
    std::shared_ptr<CleaningController> cleaningController;
    std::unique_ptr<RVC> rvc;
};

static RVCTestFixture makeTestRVC() {
    RVCTestFixture fixture;

    fixture.mockMotor = std::make_shared<MockDriveMotor>();

    fixture.cleaningController = std::make_shared<CleaningController>(
        fixture.mockMotor,
        std::make_shared<SweepingUnit>(),
        std::make_shared<DustSensor>(),
        std::make_shared<ObstacleSensor>()
    );

    fixture.rvc = std::make_unique<RVC>(fixture.cleaningController);

    return fixture;
}

// ============================================================
// 로컬 테스트: Mock 객체 사용
// USE_REAL_DEVICE를 정의하지 않으면 이 테스트가 사용됨
// ============================================================

#ifndef USE_REAL_DEVICE

TEST(RVCTest, Constructor_WhenCleaningControllerIsNull_ThrowsException) {
    EXPECT_THROW(
        RVC rvc(nullptr),
        std::invalid_argument
    );
}

TEST(RVCTest, StartCleaning_WhenPowerIsOff_DoesNothing) {
    auto fixture = makeTestRVC();

    fixture.rvc->startCleaning();

    EXPECT_EQ(fixture.mockMotor->totalCallCount(), 0);
}

TEST(RVCTest, PowerOff_WhenPowerIsOff_DoesNothing) {
    auto fixture = makeTestRVC();

    fixture.rvc->powerOff();

    EXPECT_EQ(fixture.mockMotor->stopCallCount, 0);
    EXPECT_EQ(fixture.mockMotor->totalCallCount(), 0);
}

TEST(RVCTest, PowerOn_WhenPowerIsOff_DoesNotThrow) {
    auto fixture = makeTestRVC();

    EXPECT_NO_THROW(fixture.rvc->powerOn());
}

TEST(RVCTest, PowerOn_WhenCalledTwice_DoesNotThrow) {
    auto fixture = makeTestRVC();

    EXPECT_NO_THROW(fixture.rvc->powerOn());
    EXPECT_NO_THROW(fixture.rvc->powerOn());
}

TEST(RVCTest, StopCleaning_CallsMotorStopThroughCleaningController) {
    auto fixture = makeTestRVC();

    fixture.rvc->stopCleaning();

    EXPECT_EQ(fixture.mockMotor->stopCallCount, 1);
}

TEST(RVCTest, PowerOff_WhenPowerIsOn_CallsStopCleaning) {
    auto fixture = makeTestRVC();

    fixture.rvc->powerOn();
    fixture.rvc->powerOff();

    EXPECT_EQ(fixture.mockMotor->stopCallCount, 1);
}

TEST(RVCTest, PowerOff_WhenCalledTwiceAfterPowerOn_StopsOnlyOnce) {
    auto fixture = makeTestRVC();

    fixture.rvc->powerOn();
    fixture.rvc->powerOff();
    fixture.rvc->powerOff();

    EXPECT_EQ(fixture.mockMotor->stopCallCount, 1);
}

TEST(RVCTest, PowerOnStartCleaningStopCleaning_DoesNotThrow) {
    auto fixture = makeTestRVC();

    EXPECT_NO_THROW(fixture.rvc->powerOn());
    EXPECT_NO_THROW(fixture.rvc->startCleaning());
    EXPECT_NO_THROW(fixture.rvc->stopCleaning());
}

TEST(RVCTest, RepeatedPowerCycle_OneThousandTimes_DoesNotCrash) {
    for (int i = 0; i < 1000; i++) {
        auto fixture = makeTestRVC();

        EXPECT_NO_THROW(fixture.rvc->powerOn());
        EXPECT_NO_THROW(fixture.rvc->startCleaning());
        EXPECT_NO_THROW(fixture.rvc->stopCleaning());
        EXPECT_NO_THROW(fixture.rvc->powerOff());
    }
}

#else

// ============================================================
// 서버 / CI 테스트: 실제 RVC 기본 생성자 사용
// USE_REAL_DEVICE가 정의되면 이 테스트가 사용됨
// ============================================================

TEST(RVCIntegrationTest, PowerOnAndPowerOff_WithRealComponents_DoesNotThrow) {
    RVC rvc;

    EXPECT_NO_THROW(rvc.powerOn());
    EXPECT_NO_THROW(rvc.powerOff());
}

TEST(RVCIntegrationTest, StartCleaning_WhenPowerIsOff_DoesNotThrow) {
    RVC rvc;

    EXPECT_NO_THROW(rvc.startCleaning());
}

TEST(RVCIntegrationTest, PowerOnStartCleaningStopCleaning_WithRealComponents_DoesNotThrow) {
    RVC rvc;

    EXPECT_NO_THROW(rvc.powerOn());
    EXPECT_NO_THROW(rvc.startCleaning());
    EXPECT_NO_THROW(rvc.stopCleaning());
    EXPECT_NO_THROW(rvc.powerOff());
}

#endif