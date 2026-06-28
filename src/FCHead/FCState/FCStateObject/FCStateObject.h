#ifndef FC_STATE_OBJECT_H
#define FC_STATE_OBJECT_H

#include "mbed.h"

#include <string>

#include "FCState.h"
#include "FCProcessID.h"

/*!
	\brief Базовый класс проекта
    @param const string &name - приватное имя обьекта
*/
class FCStateObject 
    : public FCCondition
{
private: 
    //! указатель на владельца
    FCStateObject *_parent;

    //! имя обьекта
    string _name; 

    //! идентификационный номер обьекта
    uint8_t _id;

public:
    //! const string &name - ссылка на имя обьекта
    FCStateObject(const string &name, uint8_t id = NoID, FCStateObject *parent = nullptr) 
        : _parent{parent},
          _name{name},
          _id{id}
    {}

    virtual ~FCStateObject() = default;

    //! метод присваивания имени обьекту
    inline void setName(const string &name) { _name = name; }

    //! метод возвращающий текущее имя обьекта
    inline string& name() { return _name; }

    //! метод возвращающий идентификационный номер обьекта
    inline uint8_t id() const { return _id; }

    //! метод возвращающий указатель на владельца
    inline FCStateObject* parent() const { return _parent; }
};

#endif // FC_STATE_OBJECT_H