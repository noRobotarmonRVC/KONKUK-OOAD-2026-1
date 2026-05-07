#include "TCPNetwork.hpp"

#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <unistd.h>

#include <cstdint>
#include <stdexcept>
#include <string>

TCPNetwork::TCPNetwork(const std::string& host, int port)
    : m_host(host), m_port(port) {
    connect();
}

TCPNetwork::~TCPNetwork() {
    if (m_sock >= 0) {
        close(m_sock);
        m_sock = -1;
    }
}

void TCPNetwork::connect() {
    m_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (m_sock < 0) {
        throw std::runtime_error("socket() failed");
    }

    timeval tv{5, 0};
    setsockopt(m_sock, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv));

    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(static_cast<uint16_t>(m_port));

    if (inet_pton(AF_INET, m_host.c_str(), &addr.sin_addr) <= 0) {
        close(m_sock);
        m_sock = -1;
        throw std::runtime_error("inet_pton() failed");
    }

    if (::connect(m_sock, reinterpret_cast<sockaddr*>(&addr), sizeof(addr)) < 0) {
        close(m_sock);
        m_sock = -1;
        throw std::runtime_error("connect() failed");
    }
}

std::string TCPNetwork::request(const std::string& cmd) {
    if (m_sock < 0) {
        throw std::runtime_error("socket is not connected");
    }

    std::string msg = cmd + "\n";

    size_t sent = 0;
    while (sent < msg.size()) {
        ssize_t n = ::send(
            m_sock,
            msg.c_str() + sent,
            msg.size() - sent,
            0
        );

        if (n <= 0) {
            throw std::runtime_error("send() failed");
        }

        sent += static_cast<size_t>(n);
    }

    std::string resp;
    char ch;

    while (true) {
        ssize_t n = recv(m_sock, &ch, 1, 0);

        if (n <= 0) {
            return "";
        }

        if (ch == '\n') {
            break;
        }

        if (ch != '\r') {
            resp.push_back(ch);
        }
    }

    return resp;
}