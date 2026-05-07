#pragma once

class AbstractSweepingUnit {
public:
    virtual ~AbstractSweepingUnit() = default;

    virtual void clean(int power) = 0;
    virtual void boostMode() = 0;
    virtual void normalMode() = 0;
};