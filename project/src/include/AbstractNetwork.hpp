#pragma once

#include <string>

class AbstractNetwork {
public:
    virtual ~AbstractNetwork() = default;

    virtual void connect() = 0;
    virtual std::string request(const std::string& cmd) = 0;
};