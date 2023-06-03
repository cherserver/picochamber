#include "DHT22.h"

namespace sensor {

DHT22::DHT22(const String &name, uint8_t pin): 
    m_name(name), 
    m_dht(pin, 22),
    m_humidity(0),
    m_temperature(0) {}

DHT22::~DHT22() {}

void DHT22::Begin() {
    m_dht.begin();
}

String DHT22::Name() {
    return m_name;
}

void DHT22::UpdateReadings() {
    sensors_event_t event;

    m_dht.temperature().getEvent(&event);
    if (!isnan(event.temperature)) {
        m_temperature = event.temperature;
    }

    m_dht.humidity().getEvent(&event);
    if (!isnan(event.relative_humidity)) {
        m_humidity = event.relative_humidity;
    }
}

void DHT22::Marshal(JsonDocument& doc) {
    JsonObject data = doc.createNestedObject(m_name);
    data["temp"] = m_temperature;
    data["hum"] = m_humidity;
}

} // end of sensor namespace