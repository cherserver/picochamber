#pragma once

#include <MarshalablePeripheral.h>

enum class HeaterMode: int8_t {
    HEATER_OFF = -1,
    HEATER_35  = 0,
    HEATER_40  = 1,
    HEATER_45  = 2,
    HEATER_50  = 3,
    HEATER_55  = 4,
    HEATER_60  = 5,
    HEATER_65  = 6,
    HEATER_70  = 7
};

class Heater: public MarshalablePeripheral {
public:
    Heater(const String name, uint8_t power_button_pin, uint8_t plus_button_pin, uint8_t minus_button_pin, uint8_t mode_button_pin);

    virtual void Begin() override;
    virtual String Name() override;
    virtual void Marshal(JsonDocument&) override;

    void SetMode(HeaterMode mode);

    void KeepAlive();

    virtual ~Heater();

private:
    Heater(Heater&) = delete;
    Heater(Heater&&) = delete;
    Heater& operator=(Heater&) = delete;
    Heater& operator=(Heater&&) = delete;

    void pressButton(uint8_t pin, uint8_t times = 1);
    void switchOff();
    void reset();

    String m_name;
    uint8_t m_power_button_pin;
    uint8_t m_plus_button_pin;
    uint8_t m_minus_button_pin;
    uint8_t m_mode_button_pin;

    HeaterMode m_mode;
};