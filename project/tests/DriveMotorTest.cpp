#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <memory>
#include <string>

#include "AbstractNetwork.hpp"
#include "DriveMotor.hpp"

using ::testing::StrictMock;

// ============================================================
// MockNetwork — gmock 기반, 기존 프로젝트 패턴과 동일
// ============================================================

class MockNetwork : public AbstractNetwork {
   public:
    MOCK_METHOD(void, connect, (), (override));
    MOCK_METHOD(void, send, (const std::string& cmd), (override));
    MOCK_METHOD(std::string, request, (const std::string& cmd), (override));
};

// ============================================================
// TC-1: network=null (기본 생성자) → 모든 메서드 crash 없이 종료
// ============================================================

TEST(DriveMotorTest, DefaultConstructor_NullNetwork_AllMethodsDoNotCrash) {
    DriveMotor motor;

    EXPECT_NO_THROW(motor.moveForward());
    EXPECT_NO_THROW(motor.moveBackward());
    EXPECT_NO_THROW(motor.stop());
    EXPECT_NO_THROW(motor.rotateRight());
    EXPECT_NO_THROW(motor.rotateLeft());
}

// ============================================================
// TC-2~6: 각 메서드 → 올바른 커맨드 문자열 1회 전송
// StrictMock 사용: 예상 외 send() 호출 시 테스트 실패
// ============================================================

TEST(DriveMotorTest, MoveForward_SendsMoveForwardCommand) {
    auto mockNetwork = std::make_shared<StrictMock<MockNetwork>>();
    DriveMotor motor(mockNetwork);

    EXPECT_CALL(*mockNetwork, send("MOVE_FORWARD")).Times(1);

    motor.moveForward();
}

TEST(DriveMotorTest, MoveBackward_SendsMoveBackwardCommand) {
    auto mockNetwork = std::make_shared<StrictMock<MockNetwork>>();
    DriveMotor motor(mockNetwork);

    EXPECT_CALL(*mockNetwork, send("MOVE_BACKWARD")).Times(1);

    motor.moveBackward();
}

TEST(DriveMotorTest, Stop_SendsStopMotorCommand) {
    auto mockNetwork = std::make_shared<StrictMock<MockNetwork>>();
    DriveMotor motor(mockNetwork);

    EXPECT_CALL(*mockNetwork, send("STOP_MOTOR")).Times(1);

    motor.stop();
}

TEST(DriveMotorTest, RotateRight_SendsRotateRightCommand) {
    auto mockNetwork = std::make_shared<StrictMock<MockNetwork>>();
    DriveMotor motor(mockNetwork);

    EXPECT_CALL(*mockNetwork, send("ROTATE_RIGHT")).Times(1);

    motor.rotateRight();
}

TEST(DriveMotorTest, RotateLeft_SendsRotateLeftCommand) {
    auto mockNetwork = std::make_shared<StrictMock<MockNetwork>>();
    DriveMotor motor(mockNetwork);

    EXPECT_CALL(*mockNetwork, send("ROTATE_LEFT")).Times(1);

    motor.rotateLeft();
}

// ============================================================
// TC-8: 동일 메서드 N회 호출 → N회 전송
// ============================================================

TEST(DriveMotorTest, MoveForward_CalledMultipleTimes_SendsCommandEachTime) {
    auto mockNetwork = std::make_shared<StrictMock<MockNetwork>>();
    DriveMotor motor(mockNetwork);

    EXPECT_CALL(*mockNetwork, send("MOVE_FORWARD")).Times(3);

    motor.moveForward();
    motor.moveForward();
    motor.moveForward();
}

// ============================================================
// TC-9: 여러 메서드 혼합 호출 → 각각 정확히 1회 전송
// ============================================================

TEST(DriveMotorTest, MixedCommands_EachSentExactlyOnce) {
    auto mockNetwork = std::make_shared<StrictMock<MockNetwork>>();
    DriveMotor motor(mockNetwork);

    EXPECT_CALL(*mockNetwork, send("MOVE_FORWARD")).Times(1);
    EXPECT_CALL(*mockNetwork, send("STOP_MOTOR")).Times(1);
    EXPECT_CALL(*mockNetwork, send("ROTATE_LEFT")).Times(1);

    motor.moveForward();
    motor.stop();
    motor.rotateLeft();
}
