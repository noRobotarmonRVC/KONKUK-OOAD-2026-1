#pragma once

#include <string>

class AbstractNetwork {
   public:
    virtual ~AbstractNetwork() = default;
    virtual void connect() = 0;
    virtual void send(const std::string& cmd) = 0;
    virtual std::string request(const std::string& cmd) = 0;
};