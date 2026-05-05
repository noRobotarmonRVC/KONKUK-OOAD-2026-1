#pragma once
#include <atomic>
#include <condition_variable>
#include <mutex>
#include <queue>
#include <string>
#include <thread>

// UDP client for the RVC Simulator.
// sendAsync: fire-and-forget (for Actuators — non-blocking)
// sendSync:  blocking round-trip (for Sensors — needs return value)
class SimulatorClient {
public:
    SimulatorClient(const std::string& host, int port);
    ~SimulatorClient();

    void        sendAsync(const std::string& cmd);
    std::string sendSync(const std::string& cmd);

private:
    void        workerLoop();
    std::string udpRoundTrip(int sockfd, const std::string& cmd);

    std::string m_host;
    int         m_port;

    // Async: command queue consumed by worker thread
    std::queue<std::string> m_queue;
    std::mutex              m_queueMutex;
    std::condition_variable m_cv;
    std::atomic<bool>       m_running{false};
    std::thread             m_worker;
    int                     m_asyncSock = -1;

    // Sync: dedicated socket, mutex-protected
    std::mutex m_syncMutex;
    int        m_syncSock = -1;
};
