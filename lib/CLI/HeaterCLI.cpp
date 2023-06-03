#include "HeaterCLI.h"

#define COMMAND_NAME "set-heater-mode"

static Heater *heaterPtr = nullptr;

void processHeaterModeCommand(cmd *cmdPtr) {
    if (heaterPtr == nullptr) {
        Serial1.println("FAIL " COMMAND_NAME ": no heater found");
        return;
    }

    Command cmd(cmdPtr);
    auto val = cmd.getArgument(0).getValue();

    auto mode = HeaterMode::HEATER_OFF;
    if (val == "off") {
        mode = HeaterMode::HEATER_OFF;
    } else if (val == "35") {
        mode = HeaterMode::HEATER_35;
    } else if (val == "40") {
        mode = HeaterMode::HEATER_40;
    } else if (val == "45") {
        mode = HeaterMode::HEATER_45;
    } else if (val == "50") {
        mode = HeaterMode::HEATER_50;
    } else if (val == "55") {
        mode = HeaterMode::HEATER_55;
    } else if (val == "60") {
        mode = HeaterMode::HEATER_60;
    } else if (val == "65") {
        mode = HeaterMode::HEATER_65;
    } else if (val == "70") {
        mode = HeaterMode::HEATER_70;
    } else {
        Serial1.print("FAIL " COMMAND_NAME ": unknown mode '");
        Serial1.print(val);
        Serial1.println("', available modes: off, 35, 40, 45, 50, 55, 60, 65, 70");
        return;
    }

    heaterPtr->SetMode(mode);

    Serial1.print("OK " COMMAND_NAME " to '");
    Serial1.print(val);
    Serial1.println("'");
}

void configureHeaterCommand(SimpleCLI *cli, Heater *heater) {
    heaterPtr = heater;

    Command cmdHatchAngle = cli->addCommand(COMMAND_NAME, processHeaterModeCommand);
    cmdHatchAngle.addPositionalArgument("mode");
}
