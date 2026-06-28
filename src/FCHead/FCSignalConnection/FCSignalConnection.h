#ifndef FC_SIGNAL_CONNECTION_H
#define FC_SIGNAL_CONNECTION_H

#include "mbed.h"
#include "events/EventQueue.h"
#include "rtos/Mutex.h"
#include <mutex>
#include <array>
#include <utility>


// ============================================================================
// RAII-гард отключения (аналог QMetaObject::Connection)
// ============================================================================
class FCSignalConnection 
{
    using Disconnector = void(*)(void*, size_t);
    Disconnector _fn = nullptr;
    void* _signal = nullptr;
    size_t _id = 0;

public:
    FCSignalConnection() = default;
    
    FCSignalConnection(void* signal, size_t id, Disconnector fn)
        : _fn{fn}, _signal{signal}, _id{id} 
    {}

    void disconnect() 
    {
        if(_fn) { 
            _fn(_signal, _id); 
            _fn = nullptr; 
        }
    }

    ~FCSignalConnection() { disconnect(); }
    
    // Non-copyable, movable
    FCSignalConnection(const FCSignalConnection&) = delete;
    FCSignalConnection& operator=(const FCSignalConnection&) = delete;
    
    FCSignalConnection(FCSignalConnection&& other) noexcept
        : _fn(other._fn), _signal(other._signal), _id(other._id) 
    {
        other._fn = nullptr;
    }

    FCSignalConnection& operator=(FCSignalConnection&& other) noexcept 
    {
        if(this != &other) {
            disconnect();
            _fn = other._fn; 
            _signal = other._signal; 
            _id = other._id;
            other._fn = nullptr;
        }
        return *this;
    }

    explicit operator bool() const { return _fn != nullptr; }
};

// ============================================================================
// ШАБЛОН СИГНАЛА
// ============================================================================
template <typename Signature, size_t MaxSlots = 4>
class FCSignal;

template <typename... Args, size_t MaxSlots>
class FCSignal<void(Args...), MaxSlots> {
    using FCCallbackType = mbed::Callback<void(Args...)>;

    struct Slot {
        FCCallbackType cb;
        bool active = false;
        size_t id = 0;
    };

    std::array<Slot, MaxSlots> _slots;
    size_t next_id_ = 1;
    // ✅ ИСПРАВЛЕНО: rtos::Mutex вместо Mutex
    mutable rtos::Mutex mutex_;

    static void disconnect_impl(void* self, size_t id) 
    {
        static_cast<FCSignal<void(Args...), MaxSlots>*>(self)->remove(id);
    }

public:
    // Подключение (возвращает RAII-гард)
    FCSignalConnection connect(FCCallbackType cb) 
    {
        // ✅ ИСПРАВЛЕНО: rtos::Mutex вместо Mutex
        std::lock_guard<rtos::Mutex> lock(mutex_);
        for(auto& s : _slots) {
            if(!s.active) {
                s.cb = std::move(cb);
                s.active = true;
                s.id = next_id_++;
                return FCSignalConnection(this, s.id, disconnect_impl);
            }
        }
        return FCSignalConnection(); // Пул полон
    }

    // Ручное отключение по ID
    void disconnect(size_t id) { remove(id); }

    // Синхронная отправка
    void emit(Args... args) const {
        std::array<FCCallbackType, MaxSlots> active_cbs;
        size_t count = 0;
        {
            // ✅ ИСПРАВЛЕНО: rtos::Mutex вместо Mutex
            std::lock_guard<rtos::Mutex> lock(mutex_);
            for(const auto& s : _slots) {
                if(s.active) {
                    active_cbs[count++] = s.cb;
                }    
            }
        }
        // Вызываем вне мьютекса, чтобы избежать deadlocks
        for(size_t i = 0; i < count; ++i) {
            if (active_cbs[i]) active_cbs[i](args...);
        }
    }

    // Асинхронная отправка через EventQueue (потокобезопасно)
    void emit_async(events::EventQueue& q, Args... args) const 
    {
        q.call([this, args...]() { this->emit(args...); });
    }

    // Очистка всех слотов
    void clear() 
    {
        // ✅ ИСПРАВЛЕНО: rtos::Mutex вместо Mutex
        std::lock_guard<rtos::Mutex> lock(mutex_);
        for(auto& s : _slots) {
            s.active = false;
            s.cb = nullptr;
        }
    }

private:
    void remove(size_t id) 
    {
        // ✅ ИСПРАВЛЕНО: rtos::Mutex вместо Mutex
        std::lock_guard<rtos::Mutex> lock(mutex_);
        for(auto& s : _slots) {
            if(s.active && s.id == id) {
                s.active = false;
                s.cb = nullptr;
                return;
            }
        }
    }
};

#endif // FC_SIGNAL_CONNECTION_H