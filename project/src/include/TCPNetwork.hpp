#pragma once

#include <string>

#include "AbstractNetwork.hpp"

class TCPNetwork : public AbstractNetwork {
   public:
    TCPNetwork(const std::string& host, int port);
    ~TCPNetwork();
    void connect() override;
    void send(const std::string& cmd) override;
    std::string request(const std::string& cmd) override;
};