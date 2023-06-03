#include "Heater.h"

const unsigned long pressDurationMs  = 200;
const unsigned long releaseTimeoutMs = 100;

Heater::Heater(const String name, uint8_t power_button_pin, uint8_t plus_button_pin, uint8_t minus_button_pin, uint8_t mode_button_pin):
    m_name(name)
    , m_power_button_pin(power_button_pin)
    , m_plus_button_pin(plus_button_pin)
    , m_minus_button_pin(minus_button_pin)
    , m_mode_button_pin(mode_button_pin)
    , m_mode(HeaterMode::HEATER_OFF) {}

Heater::~Heater() {}

void Heater::Begin() {
    pinMode(m_power_button_pin, OUTPUT);
    digitalWrite(m_power_button_pin, LOW);

    pinMode(m_plus_button_pin, OUTPUT);
    pinMode(m_minus_button_pin, OUTPUT);
    pinMode(m_mode_button_pin, OUTPUT);
}

String Heater::Name() {
    return m_name;
}

void Heater::pressButton(uint8_t pin, uint8_t times) {
    for (uint8_t i = 0; i < times; ++i) {
        delay(releaseTimeoutMs);

        digitalWrite(pin, HIGH);
        delay(pressDurationMs);

        digitalWrite(pin, LOW);
    }
}

void Heater::switchOff() {
    if (m_mode == HeaterMode::HEATER_OFF) {
		return;
	}

	pressButton(m_power_button_pin);

	m_mode = HeaterMode::HEATER_OFF;
}

void Heater::reset() {
    switchOff();

	pressButton(m_power_button_pin);
	pressButton(m_mode_button_pin);

	m_mode = HeaterMode::HEATER_35;
}

void Heater::SetMode(HeaterMode mode) {
    typedef std::underlying_type<HeaterMode>::type countType;
    countType count = static_cast<countType>(mode);

    if (count < 0) {
        switchOff();
    } else {
        reset();
        pressButton(m_plus_button_pin, count);
    }

    m_mode = mode;
}

void Heater::KeepAlive() {
    SetMode(m_mode);
}

void Heater::Marshal(JsonDocument &doc) {
    JsonObject data = doc.createNestedObject(m_name);
    
    String mode;
    switch (m_mode) {
    case HeaterMode::HEATER_OFF:
        mode = "off";
        break;
    case HeaterMode::HEATER_35:
        mode = "35";
        break;
    case HeaterMode::HEATER_40:
        mode = "40";
        break;
    case HeaterMode::HEATER_45:
        mode = "45";
        break;
    case HeaterMode::HEATER_50:
        mode = "50";
        break;
    case HeaterMode::HEATER_55:
        mode = "55";
        break;
    case HeaterMode::HEATER_60:
        mode = "60";
        break;
    case HeaterMode::HEATER_65:
        mode = "65";
        break;
    case HeaterMode::HEATER_70:
        mode = "70";
        break;
    default:
        mode = "unknown";
        break;
    }

    data["mode"] = mode;
}