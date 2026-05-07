#pragma once

#include <string>

#include "AbstractNetwork.hpp"

class TCPNetwork : public AbstractNetwork {
   public:
    TCPNetwork(const std::string& host, int port);
    ~TCPNetwork();
    void connect() override;
    std::string request(const std::string& cmd) override;

   private:
    std::string m_host;
    int         m_port;
    int         m_sock = -1;
};