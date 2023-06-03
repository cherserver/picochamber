#include <Config.h>
#include <Arduino.h>
#include <ArduinoJSON.h>
#include <TaskScheduler.h>
#include <SimpleCLI.h>

#include <MarshalablePeripheral.h>
#include <DHT22.h>
#include <Fan.h>
#include <Hatch.h>
#include <Heater.h>

#include <AutoReportCLI.h>
#include <FanCLI.h>
#include <HatchCLI.h>
#include <HeaterCLI.h>

Scheduler taskScheduler;
SimpleCLI cli;

sensor::DHT22 dryerDHT("dryer_th", DHT22_PIN_DRYER);
sensor::DHT22 innerDHT("inner_th", DHT22_PIN_INNER);
sensor::DHT22 outerDHT("outer_th", DHT22_PIN_OUTER);

Fan dryerFan("dryer_fan", FAN_PWM_PIN_DRYER, FAN_RPM_PIN_DRYER);
Fan outerFan("outer_fan", FAN_PWM_PIN_OUTER, FAN_RPM_PIN_OUTER);
Fan piFan("pi_fan", FAN_PWM_PIN_PI, FAN_RPM_PIN_PI);
Fan printerFan("printer_fan", FAN_PWM_PIN_PRINTER, FAN_RPM_PIN_PRINTER);

Hatch hatch("hatch", HATCH_PIN);
Heater heater("heater", HEATER_BUTTON_PIN_POWER, HEATER_BUTTON_PIN_PLUS, HEATER_BUTTON_PIN_MINUS, HEATER_BUTTON_PIN_MODE);

MarshalablePeripheral* peripherals[] = {
  &dryerDHT
  , &innerDHT
  , &outerDHT

  , &dryerFan
  , &outerFan
  , &piFan
  , &printerFan

  , &heater
  , &hatch
};

Fan* fans[] = {
  &dryerFan
  , &outerFan
  , &piFan
  , &printerFan
};
FanCliProcessor<sizeof(fans)/sizeof(Fan*)> fanProcessor(&fans);

sensor::DHT22* dhts[] = {
  &dryerDHT
  , &innerDHT
  , &outerDHT
};

void cliErrorCallback(cmd_error* errorPtr);
void sendSensorsData();
void readFansData();
void readDHTsData();
void keepHeaterAlive();

Task readDHTs(1 * TASK_SECOND, TASK_FOREVER, readDHTsData, &taskScheduler, true);
Task readFans(1 * TASK_SECOND, TASK_FOREVER, readFansData, &taskScheduler, true);
Task sendData(1 * TASK_SECOND, TASK_FOREVER, sendSensorsData, &taskScheduler, false);
Task keepHeater(3 * TASK_HOUR, TASK_FOREVER, keepHeaterAlive, &taskScheduler, true);

void setup() {
  Serial1.begin(SERIAL_SPEED);
  Serial1.println("#STARTING...");

  for (const auto& peripheral : peripherals) {
    Serial1.print("#  Starting '");
    Serial1.print(peripheral->Name());
    Serial1.println("'");
    peripheral->Begin();
  }

  cli.setOnError(cliErrorCallback);

  configureAutoReportCommand(&cli, &sendData);
  fanProcessor.ConfigureFanPowerCommand(&cli);
  configureHatchCommand(&cli, &hatch);
  configureHeaterCommand(&cli, &heater);

  Serial1.println("#STARTED");
}

void loop() {
  if (Serial1.available()) {
    String input = Serial1.readStringUntil('\n');
    cli.parse(input);
  }

  taskScheduler.execute();
}

void cliErrorCallback(cmd_error* errorPtr) {
    CommandError e(errorPtr);

    Serial1.println("ERROR: " + e.toString());

    if (e.hasCommand()) {
        Serial1.println("Did you mean? " + e.getCommand().toString());
    } else {
        Serial1.println(cli.toString());
    }
}

void sendSensorsData() {
  DynamicJsonDocument doc(2048);
  for (const auto& peripheral : peripherals) {
    peripheral->Marshal(doc);
  }

  serializeJson(doc, Serial1);
  Serial1.println();
}

void readFansData() {
  for (const auto& fan : fans) {
    fan->UpdateReadings();
  }
}

void readDHTsData() {
  for (const auto& dht : dhts) {
    dht->UpdateReadings();
  }
}

void keepHeaterAlive() {
  heater.KeepAlive();
}