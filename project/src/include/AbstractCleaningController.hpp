#pragma once

class AbstractCleaningController {
public:
    virtual ~AbstractCleaningController() = default;

    virtual void run() = 0;
    virtual void stop() = 0;

    virtual void turnOnDeviceComponents() = 0;
    virtual void turnOffDeviceComponents() = 0;

    virtual bool isCleaning() const = 0;
};