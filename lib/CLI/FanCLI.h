#include <Fan.h>
#include <SimpleCLI.h>

#define FAN_POWER_COMMAND_NAME "set-fan-power"

void processPowerCommand(cmd *cmdPtr);

template<unsigned FANCOUNT>
class FanCliProcessor {
public:
    FanCliProcessor(Fan* (*fans)[FANCOUNT]);
    void ConfigureFanPowerCommand(SimpleCLI *cli);
private:
    void ProcessPowerCommand(cmd *cmdPtr);
    friend void processPowerCommand(cmd *cmdPtr);
    Fan* (*m_fans)[FANCOUNT];
};

static std::function<void(cmd*)> staticCallback = nullptr;

template<unsigned FANCOUNT>
inline FanCliProcessor<FANCOUNT>::FanCliProcessor(Fan* (*fans)[FANCOUNT]): m_fans(fans) {}

template<unsigned FANCOUNT>
static Fan* (*m_fans)[FANCOUNT] = nullptr;

template<unsigned FANCOUNT>
inline void FanCliProcessor<FANCOUNT>::ConfigureFanPowerCommand(SimpleCLI *cli) {
    staticCallback = std::bind(&FanCliProcessor::ProcessPowerCommand, this, std::placeholders::_1);
    Command cmdFanPower = cli->addCommand(FAN_POWER_COMMAND_NAME, processPowerCommand);
    cmdFanPower.addPositionalArgument("name");
    cmdFanPower.addPositionalArgument("percent");
}

void processPowerCommand(cmd *cmdPtr) {
    if (staticCallback == nullptr) {
        Serial1.println("FAIL " FAN_POWER_COMMAND_NAME ": no fans found");
        return;
    }

    staticCallback(cmdPtr);
}

template<unsigned FANCOUNT>
inline void FanCliProcessor<FANCOUNT>::ProcessPowerCommand(cmd *cmdPtr) {
    Command cmd(cmdPtr);
    auto name = cmd.getArgument(0).getValue();
    auto val = cmd.getArgument(1).getValue().toInt();
    if (val < 0 || val > 100) {
        Serial1.print("FAIL " FAN_POWER_COMMAND_NAME ": bad argument value '");
        Serial1.print(val);
        Serial1.println("', must be between 0 and 100");
        return;
    }

    for (const auto& fan : *m_fans) {
        if (fan->Name() == name) {
            fan->SetPowerPercent(val);

            Serial1.print("OK " FAN_POWER_COMMAND_NAME " '");
            Serial1.print(name);
            Serial1.print("' to '");
            Serial1.print(val);
            Serial1.println("'");
            return;
        }
    }

    Serial1.print("FAIL " FAN_POWER_COMMAND_NAME ": unknown fan name '");
    Serial1.print(name);
    Serial1.print("', available fans: ");

    bool first = true;
    for (const auto& fan : *m_fans) {
        if (first) {
            first = false;
        } else {
            Serial1.print(", ");
        }

        Serial1.print("'");
        Serial1.print(fan->Name());
        Serial1.print("'");
    }

    Serial1.println();
}
