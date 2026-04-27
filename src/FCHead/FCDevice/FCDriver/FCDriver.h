#ifndef FC_DRIVER_H
#define FC_DRIVER_H

#include "FCStateObject.h"

class FCDriver 
    : public FCStateObject 
{
public:
    FCDriver(const string &name, const uint32_t id = NoID, FCStateObject *parent = nullptr) 
        : FCStateObject(name, id, parent) 
    {};
};

#endif // FC_DRIVER_H
