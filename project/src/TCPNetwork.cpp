#include "TCPNetwork.hpp"

#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdexcept>

TCPNetwork::TCPNetwork(const std::string& host, int port)
    : m_host(host), m_port(port)
{
    connect();
}

TCPNetwork::~TCPNetwork() {
    if (m_sock >= 0) { close(m_sock); m_sock = -1; }
}

void TCPNetwork::connect() {
    m_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (m_sock < 0) throw std::runtime_error("socket() failed");

    struct timeval tv{5, 0};
    setsockopt(m_sock, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv));

    struct sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_port   = htons(static_cast<uint16_t>(m_port));
    inet_pton(AF_INET, m_host.c_str(), &addr.sin_addr);

    if (::connect(m_sock, reinterpret_cast<struct sockaddr*>(&addr), sizeof(addr)) < 0) {
        close(m_sock);
        m_sock = -1;
        throw std::runtime_error("connect() failed");
    }
}

void TCPNetwork::send(const std::string& cmd) {
    std::string msg = cmd + "\n";
    ::send(m_sock, msg.c_str(), msg.size(), 0);
    char buf[64];
    recv(m_sock, buf, sizeof(buf) - 1, 0);
}

std::string TCPNetwork::request(const std::string& cmd) {
    std::string msg = cmd + "\n";
    ::send(m_sock, msg.c_str(), msg.size(), 0);

    char buf[256];
    ssize_t n = recv(m_sock, buf, sizeof(buf) - 1, 0);
    if (n <= 0) return "";

    buf[n] = '\0';
    std::string resp(buf, n);
    while (!resp.empty() && (resp.back() == '\n' || resp.back() == '\r'))
        resp.pop_back();
    return resp;
}
