#pragma once

#include <MarshalablePeripheral.h>

class Hatch: public MarshalablePeripheral {
public:
    Hatch(const String name, uint8_t servo_pin);

    virtual void Begin() override;
    virtual String Name() override;
    virtual void Marshal(JsonDocument&) override;

    void SetAngle(uint8_t angle);

    virtual ~Hatch();

private:
    Hatch(Hatch&) = delete;
    Hatch(Hatch&&) = delete;
    Hatch& operator=(Hatch&) = delete;
    Hatch& operator=(Hatch&&) = delete;

    String m_name;
    uint8_t m_servo_pin;
    uint8_t m_servo_index;

    uint8_t m_angle;
};