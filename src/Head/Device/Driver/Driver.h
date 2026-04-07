#ifndef DRIVER_H
#define DRIVER_H

#include "Object/Object.h"

class Driver 
    : public Object 
{
public:
    Driver(const string &name, const uint32_t id = NoID, Object *parent = nullptr) 
        : Object(name, id, parent) 
    {};
};

#endif // DRIVER_H