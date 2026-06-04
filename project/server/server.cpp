#include <arpa/inet.h>
#include <csignal>
#include <cstring>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <unistd.h>
#include <vector>

static volatile bool g_running = true;

static void sigHandler(int) {
    g_running = false;
}

// 방향: 0=UP, 1=RIGHT, 2=DOWN, 3=LEFT
enum Direction {
    UP = 0,
    RIGHT = 1,
    DOWN = 2,
    LEFT = 3
};

struct Position {
    int y;
    int x;
};

class RvcSimulator {
private:
    static std::vector<std::string> map;

    Position pos{1, 1};
    Direction dir = RIGHT;

    bool cleanerOn = false;
    bool boostMode = false;
    bool dustSensorOn = true;
    bool obstacleSensorOn = true;

private:
    bool isObstacle(int y, int x) const {
        return map[y][x] == 'O';
    }

    void cleanCurrentCell() {
        if (map[pos.y][pos.x] == '.') {
            map[pos.y][pos.x] = ' ';
        }
    }

    Position nextPosition(Direction d) const {
        int dy = 0;
        int dx = 0;

        if (d == UP) dy = -1;
        else if (d == RIGHT) dx = 1;
        else if (d == DOWN) dy = 1;
        else if (d == LEFT) dx = -1;

        return {pos.y + dy, pos.x + dx};
    }

    Direction leftOf(Direction d) const {
        return static_cast<Direction>((d + 3) % 4);
    }

    Direction rightOf(Direction d) const {
        return static_cast<Direction>((d + 1) % 4);
    }

    Direction backOf(Direction d) const {
        return static_cast<Direction>((d + 2) % 4);
    }

public:
    std::string handleCommand(const std::string& cmd) {
        if (cmd == "MOVE_FORWARD") {
            Position next = nextPosition(dir);
            if (!isObstacle(next.y, next.x)) {
                pos = next;
                cleanCurrentCell();
            }
            return "OK";
        }

        if (cmd == "MOVE_BACKWARD") {
            Position next = nextPosition(backOf(dir));
            if (!isObstacle(next.y, next.x)) {
                pos = next;
                cleanCurrentCell();
            }
            return "OK";
        }

        if (cmd == "ROTATE_LEFT") {
            dir = leftOf(dir);
            return "OK";
        }

        if (cmd == "ROTATE_RIGHT") {
            dir = rightOf(dir);
            return "OK";
        }

        if (cmd == "STOP_MOTOR") {
            return "OK";
        }

        if (cmd == "CLEANER_ON") {
            cleanerOn = true;
            return "OK";
        }

        if (cmd == "CLEANER_OFF") {
            cleanerOn = false;
            return "OK";
        }

        if (cmd == "BOOST_MODE") {
            boostMode = true;
            std::cout << "BOOST MODE ON\n";
            return "OK";
        }

        if (cmd == "NORMAL_MODE") {
            boostMode = false;
            std::cout << "NORMAL MODE ON\n";
            return "OK";
        }

        if (cmd == "DUST_SENSOR_ON") {
            dustSensorOn = true;
            return "OK";
        }

        if (cmd == "DUST_SENSOR_OFF") {
            dustSensorOn = false;
            return "OK";
        }

        if (cmd == "OBSTACLE_SENSOR_ON") {
            obstacleSensorOn = true;
            return "OK";
        }

        if (cmd == "OBSTACLE_SENSOR_OFF") {
            obstacleSensorOn = false;
            return "OK";
        }

        if (cmd == "FIND_DUST") {
            if (!dustSensorOn) {
                return "DUST 0";
            }

            bool hasDust = map[pos.y][pos.x] == '.';

            if (hasDust && cleanerOn) {
                map[pos.y][pos.x] = ' ';
            }

            return hasDust ? "DUST 1" : "DUST 0";
        }

        if (cmd == "FIND_OBSTACLE") {
            if (!obstacleSensorOn) {
                return "OBSTACLE 0 0 0 0";
            }

            Position front = nextPosition(dir);
            Position left = nextPosition(leftOf(dir));
            Position right = nextPosition(rightOf(dir));
            Position back = nextPosition(backOf(dir));

            int f = isObstacle(front.y, front.x) ? 1 : 0;
            int l = isObstacle(left.y, left.x) ? 1 : 0;
            int r = isObstacle(right.y, right.x) ? 1 : 0;
            int b = isObstacle(back.y, back.x) ? 1 : 0;

            std::ostringstream oss;
            oss << "OBSTACLE " << f << " " << l << " " << r << " " << b;
            return oss.str();
        }

        if (cmd == "PRINT_MAP") {
            printMap();
            return "OK";
        }

        return "ERROR UNKNOWN_COMMAND";
    }

    void printMap() const {
        std::cout << "\n==== MAP ====\n";

        for (int y = 0; y < static_cast<int>(map.size()); y++) {
            for (int x = 0; x < static_cast<int>(map[y].size()); x++) {
                if (y == pos.y && x == pos.x) {
                    if (dir == UP) std::cout << '^';
                    else if (dir == RIGHT) std::cout << '>';
                    else if (dir == DOWN) std::cout << 'v';
                    else if (dir == LEFT) std::cout << '<';
                } else {
                    std::cout << map[y][x];
                }
            }
            std::cout << '\n';
        }

        std::cout << "=============\n";
    }
};

std::vector<std::string> RvcSimulator::map = {
    "OOOOOOOOOOOOOOOOOOOO",
    "O   .      O      OO",
    "O OOO  .  O   O   OO",
    "O     O      .    OO",
    "O .        O      OO",
    "O      .          OO",
    "O   O       .     OO",
    "O       O         OO",
    "O .        .      OO",
    "OOOOOOOOOOOOOOOOOOOO"
};

static std::string trimLine(std::string s) {
    while (!s.empty() && (s.back() == '\n' || s.back() == '\r')) {
        s.pop_back();
    }
    return s;
}

class TcpServer {
private:
    int port;
    int serverFd = -1;
    RvcSimulator simulator;

public:
    explicit TcpServer(int port) : port(port) {}

    ~TcpServer() {
        if (serverFd != -1) {
            close(serverFd);
        }
    }

    void start() {
        serverFd = socket(AF_INET, SOCK_STREAM, 0);
        if (serverFd < 0) {
            throw std::runtime_error("socket() failed");
        }

        int opt = 1;
        setsockopt(serverFd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

        sockaddr_in addr{};
        addr.sin_family = AF_INET;
        addr.sin_addr.s_addr = INADDR_ANY;
        addr.sin_port = htons(port);

        if (bind(serverFd, reinterpret_cast<sockaddr*>(&addr), sizeof(addr)) < 0) {
            throw std::runtime_error("bind() failed");
        }

        if (listen(serverFd, 5) < 0) {
            throw std::runtime_error("listen() failed");
        }

        std::cout << "RVC TCP Server started on port " << port << '\n';

        while (g_running) {
            sockaddr_in clientAddr{};
            socklen_t clientLen = sizeof(clientAddr);

            int clientFd = accept(
                serverFd,
                reinterpret_cast<sockaddr*>(&clientAddr),
                &clientLen
            );

            if (clientFd < 0) {
                if (!g_running) break;
                std::cerr << "accept() failed\n";
                continue;
            }

            std::cout << "Client connected\n";
            handleClient(clientFd);
            close(clientFd);
            std::cout << "Client disconnected\n";
        }
    }

private:
    void handleClient(int clientFd) {
        char buffer[1024];

        while (g_running) {
            std::memset(buffer, 0, sizeof(buffer));

            ssize_t len = recv(clientFd, buffer, sizeof(buffer) - 1, 0);

            if (len <= 0) {
                break;
            }

            std::string command = trimLine(std::string(buffer, len));
            std::cout << "[REQUEST] " << command << '\n';

            std::string response = simulator.handleCommand(command);
            response += "\n";

            send(clientFd, response.c_str(), response.size(), 0);

            std::cout << "[RESPONSE] " << response;
            simulator.printMap();
        }
    }
};

int main() {
    std::signal(SIGINT, sigHandler);

    try {
        TcpServer server(9000);
        server.start();
    } catch (const std::exception& e) {
        std::cerr << "Server error: " << e.what() << '\n';
        return 1;
    }

    return 0;
}