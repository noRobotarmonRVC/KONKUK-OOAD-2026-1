#include <gtest/gtest.h>

#include <array>
#include <memory>
#include <string>
#include <vector>

#include "AbstractDriveMotor.hpp"
#include "AbstractNetwork.hpp"
#include "DriveController.hpp"
#include "DriveMotor.hpp"

// index rule: [0]=front, [1]=left, [2]=right, [3]=back

// ============================================================
// MockDriveMotor — DriveController 단위 테스트용
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
// MockNetwork — DriveMotor CI 테스트용
// 실제 소켓 없이 전송된 커맨드를 캡처
// ============================================================

class MockNetwork : public AbstractNetwork {
   public:
    std::vector<std::string> sentCommands;
    bool connectCalled = false;

    void connect() override { connectCalled = true; }
    void send(const std::string& cmd) override { sentCommands.push_back(cmd); }
    std::string request(const std::string& cmd) override { return ""; }
};

// ============================================================
// 로컬 테스트: MockDriveMotor 사용 (DriveController 단위 테스트)
// USE_REAL_DEVICE 미정의 시 사용
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

TEST(DriveControllerTest, Avoid_FrontBlocked_LeftOpen_RightAndBackBlocked_RotatesLeft) {
    auto motor = std::make_shared<MockDriveMotor>();
    DriveController ctrl(motor);

    ctrl.avoid({1, 0, 1, 1});

    EXPECT_EQ(motor->rotateLeftCallCount, 1);
    EXPECT_EQ(motor->rotateRightCallCount, 0);
    EXPECT_EQ(motor->moveBackwardCallCount, 0);
    EXPECT_EQ(motor->stopCallCount, 0);
}

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

TEST(DriveControllerTest, Avoid_FrontLeftBackBlocked_RightOpen_RotatesRight) {
    auto motor = std::make_shared<MockDriveMotor>();
    DriveController ctrl(motor);

    ctrl.avoid({1, 1, 0, 1});

    EXPECT_EQ(motor->rotateRightCallCount, 1);
    EXPECT_EQ(motor->rotateLeftCallCount, 0);
    EXPECT_EQ(motor->moveBackwardCallCount, 0);
    EXPECT_EQ(motor->stopCallCount, 0);
}

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
// CI 통합 테스트: MockNetwork + DriveMotor 사용
// DriveController → DriveMotor → Network 전 체인 검증
// USE_REAL_DEVICE 정의 시 사용 (실제 소켓 연결 불필요)
// ============================================================

// front 막힘, left 열림 → ROTATE_LEFT 커맨드 전송
TEST(DriveControllerCITest, Avoid_FrontBlocked_LeftOpen_SendsRotateLeft) {
    auto network = std::make_shared<MockNetwork>();
    auto motor = std::make_shared<DriveMotor>(network);
    DriveController ctrl(motor);

    ctrl.avoid({1, 0, 0, 0});

    ASSERT_EQ(network->sentCommands.size(), 1u);
    EXPECT_EQ(network->sentCommands[0], "ROTATE_LEFT");
}

// front·left 막힘, right 열림 → ROTATE_RIGHT 커맨드 전송
TEST(DriveControllerCITest, Avoid_FrontLeftBlocked_RightOpen_SendsRotateRight) {
    auto network = std::make_shared<MockNetwork>();
    auto motor = std::make_shared<DriveMotor>(network);
    DriveController ctrl(motor);

    ctrl.avoid({1, 1, 0, 0});

    ASSERT_EQ(network->sentCommands.size(), 1u);
    EXPECT_EQ(network->sentCommands[0], "ROTATE_RIGHT");
}

// front·left·right 막힘, back 열림 → MOVE_BACKWARD, STOP_MOTOR, ROTATE_LEFT 순서로 전송
TEST(DriveControllerCITest, Avoid_FrontLeftRightBlocked_BackOpen_SendsCorrectSequence) {
    auto network = std::make_shared<MockNetwork>();
    auto motor = std::make_shared<DriveMotor>(network);
    DriveController ctrl(motor);

    ctrl.avoid({1, 1, 1, 0});

    ASSERT_EQ(network->sentCommands.size(), 3u);
    EXPECT_EQ(network->sentCommands[0], "MOVE_BACKWARD");
    EXPECT_EQ(network->sentCommands[1], "STOP_MOTOR");
    EXPECT_EQ(network->sentCommands[2], "ROTATE_LEFT");
}

// front 미차단 → 예외, 네트워크 커맨드 없음
TEST(DriveControllerCITest, Avoid_FrontNotBlocked_ThrowsAndSendsNothing) {
    auto network = std::make_shared<MockNetwork>();
    auto motor = std::make_shared<DriveMotor>(network);
    DriveController ctrl(motor);

    EXPECT_THROW(ctrl.avoid({0, 0, 0, 0}), std::invalid_argument);

    EXPECT_TRUE(network->sentCommands.empty());
}

// 사방 막힘 → 예외, 네트워크 커맨드 없음
TEST(DriveControllerCITest, Avoid_AllSidesBlocked_ThrowsAndSendsNothing) {
    auto network = std::make_shared<MockNetwork>();
    auto motor = std::make_shared<DriveMotor>(network);
    DriveController ctrl(motor);

    EXPECT_THROW(ctrl.avoid({1, 1, 1, 1}), std::invalid_argument);

    EXPECT_TRUE(network->sentCommands.empty());
}

// moveForward → MOVE_FORWARD 커맨드 전송
TEST(DriveControllerCITest, MoveForward_SendsMoveForwardCommand) {
    auto network = std::make_shared<MockNetwork>();
    auto motor = std::make_shared<DriveMotor>(network);
    DriveController ctrl(motor);

    ctrl.moveForward();

    ASSERT_EQ(network->sentCommands.size(), 1u);
    EXPECT_EQ(network->sentCommands[0], "MOVE_FORWARD");
}

// stop → STOP_MOTOR 커맨드 전송
TEST(DriveControllerCITest, Stop_SendsStopMotorCommand) {
    auto network = std::make_shared<MockNetwork>();
    auto motor = std::make_shared<DriveMotor>(network);
    DriveController ctrl(motor);

    ctrl.stop();

    ASSERT_EQ(network->sentCommands.size(), 1u);
    EXPECT_EQ(network->sentCommands[0], "STOP_MOTOR");
}

#endif
