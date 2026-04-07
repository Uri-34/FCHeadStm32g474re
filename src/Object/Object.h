#ifndef OBJECT_H
#define OBJECT_H

#include "mbed.h"

#include <string>

#include "State/State.h"
#include "Object/ProcessID.h"
#include "Terminal/Terminal.h"

/*!
	\brief Базовый класс проекта
    @param const string &name - приватное имя обьекта
*/
class Object 
    : public State
{
private: 
    //! указатель на владельца
     Object *_parent;

    //! имя обьекта
    string _name; 

    //! идентификационный номер обьекта
    uint32_t _id;

    public:
    //! const string &name - ссылка на имя обьекта
    Object(const string &name, const uint32_t id = NoID, Object *parent = nullptr) 
        : State(),
        _parent{parent},
        _name{name},
        _id{id}
    {}

    virtual ~Object() = default;

    //! метод присваивания имени обьекту
    inline void setName(const string &name) { _name = name; }

    //! метод возвращающий текущее имя обьекта
    inline const string& name() const { return _name; }

    //! метод возвращающий идентификационный номер обьекта
    inline uint32_t id() const { return _id; }

    //! метод возвращающий указатель на владельца
    inline Object* parent() const { return _parent; }
};

#endif // OBJECT_H