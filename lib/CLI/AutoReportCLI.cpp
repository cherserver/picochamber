#include "AutoReportCLI.h"

#define COMMAND_NAME "set-auto-report"

static Task *taskPtr = nullptr;

void processAutoReportCommand(cmd *cmdPtr) {
    if (taskPtr == nullptr) {
        Serial1.println("FAIL " COMMAND_NAME ": no task found");
        return;
    }

    Command cmd(cmdPtr);
    auto val = cmd.getArgument(0).getValue();

    if (val == "on") {
        taskPtr->enableIfNot();
    } else if (val == "off") {
        taskPtr->disable();
    } else {
        Serial1.print("FAIL " COMMAND_NAME ": unknown mode '");
        Serial1.print(val);
        Serial1.println("', available modes: on, off");
        return;
    }

    Serial1.print("OK " COMMAND_NAME " to '");
    Serial1.print(val);
    Serial1.println("'");
}

void configureAutoReportCommand(SimpleCLI *cli, Task *task) {
    taskPtr = task;

    Command cmdHatchAngle = cli->addCommand(COMMAND_NAME, processAutoReportCommand);
    cmdHatchAngle.addPositionalArgument("mode");
}