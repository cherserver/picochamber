#include "Fan.h"

const float fanPWMFrequency = 25000; // Target frequency: 25kHz, acceptable range 21kHz to 28kHz (https://noctua.at/pub/media/wysiwyg/Noctua_PWM_specifications_white_paper.pdf)
const float secondsInMinute = 60;
const float millisecondsInSecond = 1000;
const float interruptsPerRevolution = 2;

void countInterrupt(Fan* fan) {
    fan->CountInterrupt();
}

Fan::Fan(const String name, uint8_t pwm_pin, uint8_t rpm_pin): 
    m_name(name),
    m_pwm_pin(pwm_pin),
    m_rpm_pin(rpm_pin),
    m_pwm_hardware(new RP2040_PWM(pwm_pin, fanPWMFrequency, 0)),
    m_pwm(0),
    m_rpm(0),
    m_last_update(0),
    m_counter(0) {}

Fan::~Fan() {}

void Fan::Begin() {
    pinMode(m_rpm_pin, INPUT_PULLUP);
    m_pwm_hardware->setPWM();

    m_last_update = millis();
    m_counter = 0;
    attachInterrupt(digitalPinToInterrupt(m_rpm_pin), countInterrupt, RISING, this);
}

String Fan::Name() {
    return m_name;
}

void Fan::CountInterrupt() {
    m_counter++;
}

void Fan::UpdateReadings() {
    uint64_t counter = 0;
    auto lastUpdate = m_last_update;

    { // here instead of atomics
        noInterrupts();
        counter = m_counter;
        m_counter = 0;
        interrupts();
    }

    m_last_update = millis();

    const auto millisDiff = m_last_update - lastUpdate;
    m_rpm = (counter * (secondsInMinute / interruptsPerRevolution) * millisecondsInSecond) / millisDiff;
}

void Fan::SetPowerPercent(uint8_t percent) {
    m_pwm_hardware->setPWM(m_pwm_pin, fanPWMFrequency, percent);
    m_pwm = percent;
}

void Fan::Marshal(JsonDocument& doc) {
    JsonObject data = doc.createNestedObject(m_name);
    data["p"] = m_pwm;
    data["rpm"] = m_rpm;
}