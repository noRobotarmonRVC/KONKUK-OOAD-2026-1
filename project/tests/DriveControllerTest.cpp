#include <gtest/gtest.h>

#include <array>
#include <memory>
#include <string>
#include <vector>

#include "AbstractDriveMotor.hpp"
#include "DriveController.hpp"
#include "DriveMotor.hpp"

// ============================================================
// MockDriveMotor
// gmock 없이 순수 gtest만으로 만든 수동 Mock
// index rule: [0]=front, [1]=left, [2]=right, [3]=back
// ============================================================

class MockDriveMotor : public AbstractDriveMotor {
   public:
    int moveForwardCallCount = 0;
    int moveBackwardCallCount = 0;
    int stopCallCount = 0;
    int rotateRightCallCount = 0;
    int rotateLeftCallCount = 0;

    void moveForward() override { moveForwardCallCount++; }
    void moveBackward() override { moveBackwardCallCount++; }
    void stop() override { stopCallCount++; }
    void rotateRight() override { rotateRightCallCount++; }
    void rotateLeft() override { rotateLeftCallCount++; }
};

class OrderTrackingMockDriveMotor : public AbstractDriveMotor {
   public:
    std::vector<std::string> callOrder;
    void moveForward() override { callOrder.push_back("moveForward"); }
    void moveBackward() override { callOrder.push_back("moveBackward"); }
    void stop() override { callOrder.push_back("stop"); }
    void rotateRight() override { callOrder.push_back("rotateRight"); }
    void rotateLeft() override { callOrder.push_back("rotateLeft"); }
};

// ============================================================
// 로컬 테스트: Mock 객체 사용
// ============================================================

#ifndef USE_REAL_DEVICE

// --- 생성자 ---

TEST(DriveControllerTest, Constructor_NullMotor_ThrowsInvalidArgument) {
    EXPECT_THROW(DriveController(nullptr), std::invalid_argument);
}

// --- avoid: 예외 케이스 ---

TEST(DriveControllerTest, Avoid_FrontNotBlocked_ThrowsInvalidArgument) {
    auto motor = std::make_shared<MockDriveMotor>();
    DriveController ctrl(motor);

    // front=0이면 장애물이 없는 상태로 호출된 것 → 예외
    EXPECT_THROW(ctrl.avoid({0, 0, 0, 0}), std::invalid_argument);
    EXPECT_THROW(ctrl.avoid({0, 1, 1, 1}), std::invalid_argument);

    EXPECT_EQ(motor->moveForwardCallCount, 0);
    EXPECT_EQ(motor->moveBackwardCallCount, 0);
    EXPECT_EQ(motor->rotateLeftCallCount, 0);
    EXPECT_EQ(motor->rotateRightCallCount, 0);
    EXPECT_EQ(motor->stopCallCount, 0);
}

TEST(DriveControllerTest, Avoid_AllSidesBlocked_ThrowsInvalidArgument) {
    auto motor = std::make_shared<MockDriveMotor>();
    DriveController ctrl(motor);

    EXPECT_THROW(ctrl.avoid({1, 1, 1, 1}), std::invalid_argument);

    EXPECT_EQ(motor->moveBackwardCallCount, 0);
    EXPECT_EQ(motor->rotateLeftCallCount, 0);
    EXPECT_EQ(motor->rotateRightCallCount, 0);
}

// --- avoid: 정상 케이스 ---

// front 막힘, left 열림 → rotateLeft
TEST(DriveControllerTest, Avoid_FrontBlocked_LeftOpen_RotatesLeft) {
    auto motor = std::make_shared<MockDriveMotor>();
    DriveController ctrl(motor);

    ctrl.avoid({1, 0, 0, 0});

    EXPECT_EQ(motor->rotateLeftCallCount, 1);
    EXPECT_EQ(motor->rotateRightCallCount, 0);
    EXPECT_EQ(motor->moveForwardCallCount, 0);
    EXPECT_EQ(motor->moveBackwardCallCount, 0);
    EXPECT_EQ(motor->stopCallCount, 0);
}

// front·left 막힘, left 열림 → rotateLeft (right·back 무관)
TEST(DriveControllerTest, Avoid_FrontBlocked_LeftOpen_RightAndBackBlocked_RotatesLeft) {
    auto motor = std::make_shared<MockDriveMotor>();
    DriveController ctrl(motor);

    ctrl.avoid({1, 0, 1, 1});

    EXPECT_EQ(motor->rotateLeftCallCount, 1);
    EXPECT_EQ(motor->rotateRightCallCount, 0);
    EXPECT_EQ(motor->moveBackwardCallCount, 0);
    EXPECT_EQ(motor->stopCallCount, 0);
}

// front·left 막힘, right 열림 → rotateRight
TEST(DriveControllerTest, Avoid_FrontLeftBlocked_RightOpen_RotatesRight) {
    auto motor = std::make_shared<MockDriveMotor>();
    DriveController ctrl(motor);

    ctrl.avoid({1, 1, 0, 0});

    EXPECT_EQ(motor->rotateRightCallCount, 1);
    EXPECT_EQ(motor->rotateLeftCallCount, 0);
    EXPECT_EQ(motor->moveForwardCallCount, 0);
    EXPECT_EQ(motor->moveBackwardCallCount, 0);
    EXPECT_EQ(motor->stopCallCount, 0);
}

// front·left 막힘, right 열림 (back 막힘) → rotateRight
TEST(DriveControllerTest, Avoid_FrontLeftBackBlocked_RightOpen_RotatesRight) {
    auto motor = std::make_shared<MockDriveMotor>();
    DriveController ctrl(motor);

    ctrl.avoid({1, 1, 0, 1});

    EXPECT_EQ(motor->rotateRightCallCount, 1);
    EXPECT_EQ(motor->rotateLeftCallCount, 0);
    EXPECT_EQ(motor->moveBackwardCallCount, 0);
    EXPECT_EQ(motor->stopCallCount, 0);
}

// front·left·right 막힘, back 열림 → moveBackward + stop + rotateLeft
TEST(DriveControllerTest, Avoid_FrontLeftRightBlocked_BackOpen_MovesBackThenRotatesLeft) {
    auto motor = std::make_shared<MockDriveMotor>();
    DriveController ctrl(motor);

    ctrl.avoid({1, 1, 1, 0});

    EXPECT_EQ(motor->moveBackwardCallCount, 1);
    EXPECT_EQ(motor->stopCallCount, 1);
    EXPECT_EQ(motor->rotateLeftCallCount, 1);
    EXPECT_EQ(motor->rotateRightCallCount, 0);
    EXPECT_EQ(motor->moveForwardCallCount, 0);
}

// front·left·right 막힘, back 열림 → 호출 순서 보장
TEST(DriveControllerTest, Avoid_FrontLeftRightBlocked_BackOpen_CallOrderIsCorrect) {
    auto motor = std::make_shared<OrderTrackingMockDriveMotor>();
    DriveController ctrl(motor);

    ctrl.avoid({1, 1, 1, 0});

    ASSERT_EQ(motor->callOrder.size(), 3u);
    EXPECT_EQ(motor->callOrder[0], "moveBackward");
    EXPECT_EQ(motor->callOrder[1], "stop");
    EXPECT_EQ(motor->callOrder[2], "rotateLeft");
}

// --- moveForward / stop 위임 ---

TEST(DriveControllerTest, MoveForward_DelegatesToMotorOnce) {
    auto motor = std::make_shared<MockDriveMotor>();
    DriveController ctrl(motor);

    ctrl.moveForward();

    EXPECT_EQ(motor->moveForwardCallCount, 1);
    EXPECT_EQ(motor->stopCallCount, 0);
    EXPECT_EQ(motor->rotateLeftCallCount, 0);
    EXPECT_EQ(motor->rotateRightCallCount, 0);
    EXPECT_EQ(motor->moveBackwardCallCount, 0);
}

TEST(DriveControllerTest, Stop_DelegatesToMotorOnce) {
    auto motor = std::make_shared<MockDriveMotor>();
    DriveController ctrl(motor);

    ctrl.stop();

    EXPECT_EQ(motor->stopCallCount, 1);
    EXPECT_EQ(motor->moveForwardCallCount, 0);
    EXPECT_EQ(motor->moveBackwardCallCount, 0);
    EXPECT_EQ(motor->rotateLeftCallCount, 0);
    EXPECT_EQ(motor->rotateRightCallCount, 0);
}

// --- 복합 시나리오 ---

TEST(DriveControllerTest, MoveForward_CalledMultipleTimes_CountAccumulates) {
    auto motor = std::make_shared<MockDriveMotor>();
    DriveController ctrl(motor);

    ctrl.moveForward();
    ctrl.moveForward();
    ctrl.moveForward();

    EXPECT_EQ(motor->moveForwardCallCount, 3);
}

TEST(DriveControllerTest, Avoid_AfterThrow_ControllerStillUsable) {
    auto motor = std::make_shared<MockDriveMotor>();
    DriveController ctrl(motor);

    EXPECT_THROW(ctrl.avoid({0, 0, 0, 0}), std::invalid_argument);

    EXPECT_NO_THROW(ctrl.moveForward());
    EXPECT_EQ(motor->moveForwardCallCount, 1);
}

#else

// ============================================================
// 서버 / CI 테스트: 실제 DriveMotor 사용
// USE_REAL_DEVICE가 정의되면 이 테스트가 사용됨
// ============================================================

// front 막힘, left 열림 → rotateLeft
TEST(DriveControllerIntegrationTest, Avoid_FrontBlocked_LeftOpen_UsesRealMotor) {
    auto realMotor = std::make_shared<DriveMotor>();
    DriveController controller(realMotor);

    std::array<int, 4> obstacleInfo = {1, 0, 0, 0};

    testing::internal::CaptureStdout();
    controller.avoid(obstacleInfo);
    std::string output = testing::internal::GetCapturedStdout();

    EXPECT_NE(output.find("rotate left"), std::string::npos);
}

// front·left 막힘, right 열림 → rotateRight
TEST(DriveControllerIntegrationTest, Avoid_FrontLeftBlocked_RightOpen_UsesRealMotor) {
    auto realMotor = std::make_shared<DriveMotor>();
    DriveController controller(realMotor);

    std::array<int, 4> obstacleInfo = {1, 1, 0, 0};

    testing::internal::CaptureStdout();
    controller.avoid(obstacleInfo);
    std::string output = testing::internal::GetCapturedStdout();

    EXPECT_NE(output.find("rotate right"), std::string::npos);
}

#endif
