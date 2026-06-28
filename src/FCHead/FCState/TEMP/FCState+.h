#ifndef FC_STATE_H
#define FC_STATE_H

#include <cstdint>

enum class State : uint8_t 
{
    Ready = 0, 
    NotReady = 1,
    Started = 2, 
    Stopped = 3,
    Paused = 4, 
    Resumed = 5,
    Changed = 6, 
    NotChanged = 7
};

enum ErrorCode : uint8_t 
{
    None        = 0,
    Sensor      = 1,
    Comm        = 2,
    OverTemp    = 3,
    Overflow    = 4,
    Calibration = 5,
    Critical    = 15
};

// Класс-контейнер состояния (ровно 1 байт)
class FCCondition 
{
public:
    uint8_t _ready   : 1;
    uint8_t _started : 1;
    uint8_t _error   : 4;
    uint8_t _paused  : 1;
    uint8_t _changed : 1;

    FCCondition(State ready = State::NotReady, State started = State::Stopped, ErrorCode error = ErrorCode::None, 
                State paused = State::Paused, State changed = State::Changed) 
        : _ready{static_cast<uint8_t>(ready)},
          _started{static_cast<uint8_t>(started)}, 
          _error{static_cast<uint8_t>(error)}, 
          _paused{static_cast<uint8_t>(paused)}, 
          _changed{static_cast<uint8_t>(changed)} 
    {}

    bool hasError() const { return _error != static_cast<uint8_t>(ErrorCode::None); }
    ErrorCode error() const { return static_cast<ErrorCode>(_error); }
    void setError(ErrorCode code) { _error = static_cast<uint8_t>(code) & 0x0F; }
    void clearError() { _error = static_cast<uint8_t>(ErrorCode::None); }

    inline uint8_t raw() const 
    { 
        return (_ready & 0x01) | 
               ((_started & 0x01) << 1) | 
               ((_error & 0x0F) << 2) | 
               ((_paused & 0x01) << 6) | 
               ((_changed & 0x01) << 7);
    }

    inline static FCCondition fromRaw(uint8_t byte) 
    { 
        FCCondition c;
        c._ready   = byte & 0x01;
        c._started = (byte >> 1) & 0x01;
        c._error   = (byte >> 2) & 0x0F;
        c._paused  = (byte >> 6) & 0x01;
        c._changed = (byte >> 7) & 0x01;
        return c;
    }
};

static_assert(sizeof(FCCondition) == 1, "FCCondition должен быть равным 1 байту !!!");

// класс состояния объекта проекта
class FCState 
{
private:
    FCCondition _condition;

public:
    // Конструктор по умолчанию (создает состояние "не готов, не запущен, без ошибок")
    FCState() = default;

    // Конструктор с начальным условием (принимает константную ссылку, т.к. объект всего 1 байт)
    explicit FCState(const State &state) 
        : _condition{condition}
    {}

    inline FCCondition condition() const { return _condition; }
    inline uint8_t raw() const { return _condition.raw(); }

    //! установка состояния
    void set(State state) 
    {
        switch (state) {
            case State::Ready:      _condition._ready   = 1; break;
            case State::NotReady:   _condition._ready   = 0; break;
            case State::Started:    _condition._started = 1; break;
            case State::Stopped:    _condition._started = 0; break;
            case State::Paused:     _condition._paused  = 1; break;
            case State::Resumed:    _condition._paused  = 0; break;
            case State::Changed:    _condition._changed = 1; break;
            case State::NotChanged: _condition._changed = 0; break;
        }
    }

    //! проверка состояния
    bool is(State state) const
    {
        switch (state) {
            case State::Ready:      return _condition._ready   != 0;
            case State::NotReady:   return _condition._ready   == 0;
            case State::Started:    return _condition._started != 0;
            case State::Stopped:    return _condition._started == 0;
            case State::Paused:     return _condition._paused  != 0;
            case State::Resumed:    return _condition._paused  == 0;
            case State::Changed:    return _condition._changed != 0;
            case State::NotChanged: return _condition._changed == 0;
            default:                return false;
        }
    }

    inline bool hasError() const { return _condition.hasError(); }
    inline ErrorCode error() const { return _condition.error(); }
    inline void setError(ErrorCode code) { _condition.setError(code); }
    inline void clearError() { _condition.clearError(); }

    inline bool isOperational() const 
    { 
        return is(State::Ready) && is(State::Started) && is(State::Resumed) && !hasError(); 
    }
};
#endif // FC_STATE_H