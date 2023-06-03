#pragma once

#include <MarshalablePeripheral.h>
#include <DHT_U.h>

namespace sensor{

class DHT22: public MarshalablePeripheral {
public:
    DHT22(const String &name, uint8_t pin);

    virtual void Begin() override;
    virtual String Name() override;
    virtual void Marshal(JsonDocument&) override;

    void UpdateReadings();

    virtual ~DHT22();

private:
    DHT22(DHT22&) = delete;
    DHT22(DHT22&&) = delete;
    DHT22& operator=(DHT22&) = delete;
    DHT22& operator=(DHT22&&) = delete;

    String m_name;
    DHT_Unified m_dht;
    float m_humidity;
    float m_temperature;
};

} // end of sensor namespace