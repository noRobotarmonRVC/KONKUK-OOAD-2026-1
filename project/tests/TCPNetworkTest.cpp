#include <gtest/gtest.h>

#include <iostream>
#include <memory>

#include "TCPNetwork.hpp"

TEST(TCPNetwork, AllCommands) {
    std::shared_ptr<TCPNetwork> network;
    try {
        network = std::make_shared<TCPNetwork>("127.0.0.1", 9000);
        std::cout << "[OK] 시뮬레이터 연결 성공\n";
    } catch (const std::exception& e) {
        std::cerr << "[FAIL] 연결 실패: " << e.what() << "\n";
        std::cerr << "시뮬레이터가 실행 중인지 확인하세요 (port 9000)\n";
    }
    // 1. Motor
    std::cout << "Motor\n";
    std::cout << "MOVE_FORWARD : " << network->request("MOVE_FORWARD") << '\n';
    std::cout << "MOVE_LEFT : " << network->request("MOVE_LEFT") << '\n';
    std::cout << "MOVE_RIGHT : " << network->request("MOVE_RIGHT") << '\n';
    std::cout << "MOVE_BACKWARD : " << network->request("MOVE_BACKWARD") << '\n';
    std::cout << "STOP_MOTOR : " << network->request("STOP_MOTOR") << '\n';

    // 2. Cleaner

    std::cout << "Cleaner\n";
    std::cout << "CLEANER_ON : " << network->request("CLEANER_ON") << '\n';
    std::cout << "CLEANER_OFF : " << network->request("CLEANER_OFF") << '\n';
    std::cout << "BOOST_MODE : " << network->request("BOOST_MODE") << '\n';
    std::cout << "NORMAL_MODE : " << network->request("NORMAL_MODE") << '\n';

    // 3. DustSensor
    std::cout << "DustSensor\n";
    std::cout << "DUST_SENSOR_ON : " << network->request("DUST_SENSOR_ON") << '\n';
    std::cout << "DUST_SENSOR_OFF : " << network->request("DUST_SENSOR_OFF") << '\n';
    std::cout << "FIND_DUST : " << network->request("FIND_DUST") << '\n';

    // 4. ObastacleSensor
    std::cout << "ObstacleSensor\n";
    std::cout << "OBSTACLE_SENSOR_ON : " << network->request("OBSTACLE_SENSOR_ON") << '\n';
    std::cout << "OBSTACLE_SENSOR_OFF : " << network->request("OBSTACLE_SENSOR_OFF") << '\n';
    std::cout << "FIND_OBSTACLE : " << network->request("FIND_OBSTACLE") << '\n';
}