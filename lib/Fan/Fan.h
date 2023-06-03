#pragma once

#include <MarshalablePeripheral.h>
#include <RP2040_PWM.h>

class Fan: public MarshalablePeripheral {
public:
    Fan(const String name, uint8_t pwm_pin, uint8_t rpm_pin);

    virtual void Begin() override;
    virtual String Name() override;
    virtual void Marshal(JsonDocument&) override;

    void UpdateReadings();

    void SetPowerPercent(uint8_t pwm);

    virtual ~Fan();

private:
    Fan(Fan&) = delete;
    Fan(Fan&&) = delete;
    Fan& operator=(Fan&) = delete;
    Fan& operator=(Fan&&) = delete;

    void CountInterrupt();

    String m_name;
    uint8_t m_pwm_pin;
    uint8_t m_rpm_pin;
    RP2040_PWM *m_pwm_hardware;

    uint8_t m_pwm;
    uint32_t m_rpm;
    unsigned long m_last_update;
    volatile uint64_t m_counter;

    friend void countInterrupt(Fan* fan);
};