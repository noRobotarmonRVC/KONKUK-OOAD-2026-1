#include <gtest/gtest.h>

#include <array>
#include <memory>
#include <string>

#include "AbstractDriveMotor.hpp"
#include "DriveController.hpp"
#include "DriveMotor.hpp"

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
};

// ============================================================
// 로컬 테스트: Mock 객체 사용
// 빌드할 때 USE_REAL_DEVICE를 정의하지 않으면 이 테스트가 사용됨
// ============================================================

#ifndef USE_REAL_DEVICE

TEST(DriveControllerTest, Avoid_WhenFrontBlockedAndRightOpen_UsesMockMotor) {
    auto mockMotor = std::make_shared<MockDriveMotor>();
    DriveController controller(mockMotor);

    // obstacle_info: [front, right, back, left]
    std::array<int, 4> obstacleInfo = {1, 0, 0, 0};

    controller.avoid(obstacleInfo);

    EXPECT_EQ(mockMotor->stopCallCount, 1);
    EXPECT_EQ(mockMotor->rotateRightCallCount, 1);

    EXPECT_EQ(mockMotor->moveForwardCallCount, 0);
    EXPECT_EQ(mockMotor->moveBackwardCallCount, 0);
    EXPECT_EQ(mockMotor->rotateLeftCallCount, 0);
}

#else

// ============================================================
// 서버 / CI 테스트: 실제 DriveMotor 사용
// USE_REAL_DEVICE가 정의되면 이 테스트가 사용됨
// ============================================================

TEST(DriveControllerIntegrationTest, Avoid_WhenFrontBlockedAndRightOpen_UsesRealMotor) {
    auto realMotor = std::make_shared<DriveMotor>();
    DriveController controller(realMotor);

    std::array<int, 4> obstacleInfo = {1, 0, 0, 0};

    testing::internal::CaptureStdout();

    controller.avoid(obstacleInfo);

    std::string output = testing::internal::GetCapturedStdout();

    EXPECT_NE(output.find("rotate right"), std::string::npos);
}

#endif