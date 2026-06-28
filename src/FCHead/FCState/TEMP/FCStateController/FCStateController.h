#ifndef FC_STATE_CONTROLLER_H
#define FC_STATE_CONTROLLER_H

#include <tuple>
#include <type_traits>
#include <utility>
#include <cstddef>
#include "FCState.h" // Ваш исправленный файл с FCStateT и enum

/**
 * @brief Универсальный контроллер состояния = Данные (FCStateT) + Логика (FSM)
 * @tparam PrimaryState  Основной enum, которым управляет автомат (например, FCPlayState)
 * @tparam Flags         Дополнительные флаги-состояния (FCReadyState, FCErrorType и т.д.)
 * @tparam Event         Enum событий
 * @tparam MaxTrans      Макс. кол-во переходов (статический массив)
 */
template<typename PrimaryState, typename... Flags, typename Event, size_t MaxTrans = 32>
class FCStateController
{
    using Guard = bool(*)(PrimaryState from, Event evt, void* ctx);
    using Callback = void(*)(PrimaryState from, PrimaryState to, void* ctx);

    struct Transition {
        PrimaryState from; Event event; PrimaryState to;
        Guard guard; Callback onTrans;
    };

    // Данные состояния (всё в одном tuple)
    FCStateT<PrimaryState, Flags...> _data;
    
    // Внутренний автомат
    Transition _trans[MaxTrans];
    Callback   _entry[static_cast<size_t>(PrimaryState::Pause) + 1]{}; // +1 для безопасности индексов
    Callback   _exit[static_cast<size_t>(PrimaryState::Pause) + 1]{};
    PrimaryState _current{};
    size_t _tCount = 0;

public:
    FCStateController() = default;

    // ========================================================================
    // 1. ПРЯМОЙ ДОСТУП К ДАННЫМ (как к FCStateT)
    // ========================================================================
    template<typename T>
    FCStateController& set(T value) { _data.set(value); return *this; }

    template<typename T>
    T get() const noexcept { return _data.template get<T>(); }

    template<typename T>
    bool is(T value) const noexcept { return _data.template is(value); }

    // ========================================================================
    // 2. НАСТРОЙКА АВТОМАТА
    // ========================================================================
    void addState(PrimaryState s, Callback onEntry = nullptr, Callback onExit = nullptr) {
        auto idx = static_cast<size_t>(s);
        _entry[idx] = onEntry; _exit[idx] = onExit;
    }

    bool addTransition(PrimaryState from, Event evt, PrimaryState to, 
                       Guard g = nullptr, Callback onTrans = nullptr) 
    {
        if (_tCount >= MaxTrans) return false;
        _trans[_tCount++] = {from, evt, to, g, onTrans};
        return true;
    }

    // ========================================================================
    // 3. ЗАПУСК И ОБРАБОТКА
    // ========================================================================
    void start(PrimaryState initial) {
        _current = initial;
        if (_entry[static_cast<size_t>(_current)]) 
            _entry[static_cast<size_t>(_current)](_current, _current, this);
    }

    bool process(Event evt) {
        for (size_t i = 0; i < _tCount; ++i) {
            if (_trans[i].from == _current && _trans[i].event == evt) {
                // Проверка guard (если есть)
                if (_trans[i].guard && !_trans[i].guard(_current, evt, this)) 
                    return false;

                // Выход из текущего
                if (_exit[static_cast<size_t>(_current)]) 
                    _exit[static_cast<size_t>(_current)](_current, _trans[i].to, this);

                PrimaryState next = _trans[i].to;
                
                // Переход
                if (_trans[i].onTrans) _trans[i].onTrans(_current, next, this);
                _current = next;

                // Вход в новое
                if (_entry[static_cast<size_t>(next)]) 
                    _entry[static_cast<size_t>(next)](_current, next, this);

                return true;
            }
        }
        return false;
    }

    [[nodiscard]] PrimaryState currentState() const noexcept { return _current; }
};

#endif // FC_STATE_CONTROLLER_H
