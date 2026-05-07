#include <memory>
#include <iostream>
#include <string>
#include <chrono>
#include <thread>

#include <sys/select.h>
#include <unistd.h>

#include "AbstractNetwork.hpp"
#include "CleaningController.hpp"
#include "DriveMotor.hpp"
#include "DriveController.hpp"
#include "DustSensor.hpp"
#include "ObstacleSensor.hpp"
#include "RVC.hpp"
#include "SweepingUnit.hpp"
#include "SweepingController.hpp"
#include "DevicePowerManager.hpp"
#include "TCPNetwork.hpp"

bool hasInput() {
    fd_set set;
    FD_ZERO(&set);
    FD_SET(STDIN_FILENO, &set);

    timeval timeout{};
    timeout.tv_sec = 0;
    timeout.tv_usec = 0;

    return select(STDIN_FILENO + 1, &set, nullptr, nullptr, &timeout) > 0;
}

int main() {
    auto network = std::make_shared<TCPNetwork>("127.0.0.1", 9000);

    auto driveMotor = std::make_shared<DriveMotor>(network);
    auto cleaningUnit = std::make_shared<SweepingUnit>(network);
    auto dustSensor = std::make_shared<DustSensor>(network);
    auto obstacleSensor = std::make_shared<ObstacleSensor>(network);

    auto driveController = std::make_shared<DriveController>(driveMotor);
    auto sweepingController = std::make_shared<SweepingController>(cleaningUnit);

    auto devicePowerManager = std::make_shared<DevicePowerManager>(
        std::array<std::shared_ptr<DeviceComponent>, 3>{
            cleaningUnit,
            dustSensor,
            obstacleSensor
        }
    );

    auto cleaningController = std::make_unique<CleaningController>(
        driveController,
        sweepingController,
        devicePowerManager,
        dustSensor,
        obstacleSensor
    );

    RVC rvc(std::move(cleaningController));

    std::cout << "1: powerOn\n";
    std::cout << "2: startCleaning\n";
    std::cout << "3: stopCleaning\n";
    std::cout << "4: powerOff\n";
    std::cout << "q: quit\n";

    while (true) {
        if (hasInput()) {
            std::string input;
            std::getline(std::cin, input);

            if (input == "1") {
                rvc.powerOn();
                std::cout << "[OK] powerOn\n";
            } else if (input == "2") {
                rvc.startCleaning();
                std::cout << "[OK] startCleaning\n";
            } else if (input == "3") {
                rvc.stopCleaning();
                std::cout << "[OK] stopCleaning\n";
            } else if (input == "4") {
                rvc.powerOff();
                std::cout << "[OK] powerOff\n";
            } else if (input == "q") {
                std::cout << "종료합니다.\n";
                break;
            } else if (!input.empty()) {
                std::cout << "잘못된 입력: " << input << '\n';
            }
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }

    return 0;
}
