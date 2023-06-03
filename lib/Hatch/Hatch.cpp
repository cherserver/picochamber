#include "Hatch.h"
#include <RP2040_ISR_Servo.h>

const uint8_t angle0PWMValue  = 15; // lower
const uint8_t angle90PWMValue = 58; // upper
const float_t angleStep       = float_t(angle90PWMValue-angle0PWMValue) / 90;

uint8_t angleToPWM(uint8_t angle) {
	return angle0PWMValue + uint8_t(float_t(angle)*angleStep);
}

Hatch::Hatch(const String name, uint8_t pwm_pin): 
    m_name(name),
    m_servo_pin(pwm_pin),
    m_servo_index(0),
    m_angle(0) {}

Hatch::~Hatch() {}

void Hatch::Begin() {
    m_servo_index = RP2040_ISR_Servos.setupServo(m_servo_pin);
    SetAngle(m_angle);
}

String Hatch::Name() {
    return m_name;
}

void Hatch::SetAngle(uint8_t angle) {
    RP2040_ISR_Servos.setPosition(m_servo_index, angleToPWM(angle));
    m_angle = angle;
}

void Hatch::Marshal(JsonDocument& doc) {
    JsonObject data = doc.createNestedObject(m_name);
    data["angle"] = m_angle;
}