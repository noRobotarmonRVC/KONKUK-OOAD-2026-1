#pragma once

class DeviceComponent {
   protected:
    bool is_on = false;

   public:
    virtual ~DeviceComponent() = default;

    virtual bool isOn() const = 0;
    virtual void turnOn() = 0;
    virtual void turnOff() = 0;
};
