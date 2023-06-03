#include "HatchCLI.h"

#define COMMAND_NAME "set-hatch-angle"

static Hatch *hatchPtr = nullptr;

void processHatchAngleCommand(cmd *cmdPtr) {
    if (hatchPtr == nullptr) {
        Serial1.println("FAIL " COMMAND_NAME ": no hatch found");
        return;
    }

    Command cmd(cmdPtr);
    auto val = cmd.getArgument(0).getValue().toInt();
    if (val < 0 || val > 90) {
        Serial1.print("FAIL " COMMAND_NAME ": bad argument value '");
        Serial1.print(val);
        Serial1.println("', must be between 0 and 90");
        return;
    }

    hatchPtr->SetAngle(val);

    Serial1.print("OK " COMMAND_NAME " to '");
    Serial1.print(val);
    Serial1.println("'");
}

void configureHatchCommand(SimpleCLI *cli, Hatch *hatch) {
    hatchPtr = hatch;

    Command cmdHatchAngle = cli->addCommand(COMMAND_NAME, processHatchAngleCommand);
    cmdHatchAngle.addPositionalArgument("angle");
}
