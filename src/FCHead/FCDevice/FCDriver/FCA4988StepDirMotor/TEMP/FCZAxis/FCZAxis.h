#ifndef FC_Z_AXIS_H
#define FC_Z_AXIS_H

#include "FCA4988StepDirMotorWithEndStop.h"

class FCZAxis 
    : public FCA4988StepDirMotorWithEndStop
{
public:
    FCZAxis(FCI2CBus &bus, FCStateObject *parent = nullptr);

private:

};

#endif // FC_Z_AXIS_H
