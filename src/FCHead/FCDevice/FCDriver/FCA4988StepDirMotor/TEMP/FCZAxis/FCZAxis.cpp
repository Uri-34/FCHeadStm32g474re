#include "FCZAxis.h"

FCZAxis::FCZAxis(FCI2CBus &bus, FCStateObject *parent)
    : FCA4988StepDirMotorWithEndStop::FCA4988StepDirMotorWithEndStop(&bus, 0x20, 0x21, 16, PB_0, PB_1, "z", parent)
{}
