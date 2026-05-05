#include "SimulatorClient.hpp"
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <cstring>
#include <stdexcept>

static int openUdpSocket(int timeoutSec) {
    int fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (fd < 0) throw std::runtime_error("socket() failed");
    struct timeval tv{timeoutSec, 0};
    setsockopt(fd, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv));
    return fd;
}

SimulatorClient::SimulatorClient(const std::string& host, int port)
    : m_host(host), m_port(port)
{
    m_asyncSock = openUdpSocket(1);
    m_syncSock  = openUdpSocket(2);
    m_running   = true;
    m_worker    = std::thread(&SimulatorClient::workerLoop, this);
}

SimulatorClient::~SimulatorClient() {
    m_running = false;
    m_cv.notify_all();
    if (m_worker.joinable()) m_worker.join();
    if (m_asyncSock >= 0) { close(m_asyncSock); m_asyncSock = -1; }
    if (m_syncSock  >= 0) { close(m_syncSock);  m_syncSock  = -1; }
}

void SimulatorClient::sendAsync(const std::string& cmd) {
    {
        std::lock_guard<std::mutex> lock(m_queueMutex);
        m_queue.push(cmd);
    }
    m_cv.notify_one();
}

std::string SimulatorClient::sendSync(const std::string& cmd) {
    std::lock_guard<std::mutex> lock(m_syncMutex);
    return udpRoundTrip(m_syncSock, cmd);
}

void SimulatorClient::workerLoop() {
    while (true) {
        std::unique_lock<std::mutex> lock(m_queueMutex);
        m_cv.wait(lock, [this] { return !m_queue.empty() || !m_running; });

        if (!m_running && m_queue.empty()) break;

        std::string cmd = std::move(m_queue.front());
        m_queue.pop();
        lock.unlock();

        udpRoundTrip(m_asyncSock, cmd);
    }
}

std::string SimulatorClient::udpRoundTrip(int sockfd, const std::string& cmd) {
    struct sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_port   = htons(static_cast<uint16_t>(m_port));
    inet_pton(AF_INET, m_host.c_str(), &addr.sin_addr);

    std::string msg = cmd + "\n";
    sendto(sockfd, msg.c_str(), msg.size(), 0,
           reinterpret_cast<struct sockaddr*>(&addr), sizeof(addr));

    char buf[256];
    struct sockaddr_in from{};
    socklen_t fromLen = sizeof(from);
    ssize_t n = recvfrom(sockfd, buf, sizeof(buf) - 1, 0,
                         reinterpret_cast<struct sockaddr*>(&from), &fromLen);
    if (n <= 0) return "";

    buf[n] = '\0';
    std::string resp(buf, n);
    while (!resp.empty() && (resp.back() == '\n' || resp.back() == '\r'))
        resp.pop_back();
    return resp;
}
