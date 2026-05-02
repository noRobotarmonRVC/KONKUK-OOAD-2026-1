#pragma once

class DeviceComponent {
protected:
    bool isOn_ = false;

public:
    virtual ~DeviceComponent() = default;

    virtual bool isOn() const = 0;
    virtual void turnOn() = 0;
    virtual void turnOff() = 0;
};
