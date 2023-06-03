#pragma once

#include <ArduinoJson.h>

class MarshalablePeripheral {
public:
    virtual void Begin() = 0;
    virtual String Name() = 0;
    
    virtual void Marshal(JsonDocument&) = 0;
};