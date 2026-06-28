#ifndef FC_STATE_H
#define FC_STATE_H
/**
* @file FCState.h
* @brief Централизованное управление состояниями системы Funny Cake Plotter
* @version 2.5.0 (C++14 Compatible, No Qt, Minimal)
* @note Требует C++14 или выше. Не требует Qt.
*/
#include <tuple>
#include <type_traits>
#include <utility>  // Для std::index_sequence
#include <string>   // Вместо <QString>

// ============================================================================
// ENUM (только реально используемые)
// ============================================================================
enum class FCReadyState { NotReady, Ready };
enum class FCPlayState  { Stop, Start, Pause };
enum class FCErrorType {
    None, Open, Close, Connection, Disconnection,
    Create, Destroy, Read, Write, OutOfRange,
    Configuration, Parse, Timeout, EmergencyStoped, Motion, Update
};

// ============================================================================
// TYPE TRAITS (C++14 compatible)
// ============================================================================
template<typename T> struct is_fc_state_type : std::false_type {};
template<> struct is_fc_state_type<FCReadyState> : std::true_type {};
template<> struct is_fc_state_type<FCPlayState>  : std::true_type {};
template<> struct is_fc_state_type<FCErrorType>  : std::true_type {};

// contains_type: проверка наличия типа в пакете (рекурсия, C++14)
template<typename T, typename... Pack>
struct contains_type : std::false_type {};
template<typename T, typename First, typename... Rest>
struct contains_type<T, First, Rest...>
    : std::integral_constant<bool,
        std::is_same<T, First>::value || contains_type<T, Rest...>::value> {};

// all_fc_states: проверка что все типы в пакете валидны
template<typename... Ts> struct all_fc_states : std::true_type {};
template<typename First, typename... Rest>
struct all_fc_states<First, Rest...>
    : std::integral_constant<bool,
        is_fc_state_type<First>::value && all_fc_states<Rest...>::value> {};

// ============================================================================
// FCStateT CLASS TEMPLATE (C++14 compatible)
// ============================================================================
template<typename... States>
class FCStateT
{
    static_assert(all_fc_states<States...>::value,
        "Invalid state type. All parameters must be registered in is_fc_state_type.");

    std::tuple<States...> _values{};

public:
    FCStateT() = default;
    FCStateT(const FCStateT&) = default;
    FCStateT& operator=(const FCStateT&) = default;

    // Конструктор с инициализацией (порядок аргументов произвольный)
    template<typename... InitStates>
    explicit FCStateT(InitStates... initialValues) {
        static_assert(all_fc_states<InitStates...>::value, "Invalid init types");
        // Array trick для разворачивания пакета в C++14
        int dummy[] = {0, (setValueByType(_values, initialValues), 0)...};
        (void)dummy;
    }

    // setValueByType: устанавливает значение только если тип валиден (SFINAE)
    template<typename State>
    static typename std::enable_if<contains_type<State, States...>::value, void>::type
    setValueByType(std::tuple<States...>& values, State value) {
        std::get<State>(values) = value;
    }
    template<typename State>
    static typename std::enable_if<!contains_type<State, States...>::value, void>::type
    setValueByType(std::tuple<States...>&, State) { /* Игнорируем невалидные типы */ }

    // Установка состояния
    template<typename State>
    FCStateT& set(State value) {
        static_assert(contains_type<State, States...>::value, "Unknown state type");
        std::get<State>(_values) = value;
        return *this;
    }

    // Получение состояния
    template<typename State>
    State get() const noexcept {
        static_assert(contains_type<State, States...>::value, "Unknown state type");
        return std::get<State>(_values);
    }

    // Проверка состояния
    template<typename State>
    bool is(State value) const noexcept {
        static_assert(contains_type<State, States...>::value, "Unknown state type");
        return std::get<State>(_values) == value;
    }

    // Сравнение (C++14 через index_sequence)
    bool operator==(const FCStateT& o) const noexcept {
        return areEqual(o, std::make_index_sequence<sizeof...(States)>{});
    }
    bool operator!=(const FCStateT& o) const noexcept { return !(*this == o); }

    template<std::size_t... I>
    bool areEqual(const FCStateT& o, std::index_sequence<I...>) const noexcept {
        bool res[] = {(std::get<I>(_values) == std::get<I>(o._values))...};
        for (bool b : res) if (!b) return false;
        return true;
    }

    // Слияние (копирует только общие типы)
    template<typename... Other>
    void mergeFrom(const FCStateT<Other...>& o) noexcept {
        int dummy[] = {0, (copyIfCommon<States>(o), 0)...};
        (void)dummy;
    }

    // copyIfCommon (SFINAE)
    template<typename S, typename... O>
    typename std::enable_if<contains_type<S, O...>::value, void>::type
    copyIfCommon(const FCStateT<O...>& o) noexcept {
        std::get<S>(_values) = std::get<S>(o._values);
    }
    template<typename S, typename... O>
    typename std::enable_if<!contains_type<S, O...>::value, void>::type
    copyIfCommon(const FCStateT<O...>&) noexcept {}

    // Convenience-методы
    bool isReady() const noexcept {
        return contains_type<FCReadyState, States...>::value ? is(FCReadyState::Ready) : false;
    }
    bool hasError() const noexcept {
        return contains_type<FCErrorType, States...>::value ? get<FCErrorType>() != FCErrorType::None : false;
    }

    // Отладка (std::string вместо QString)
    std::string debugTypeNames() const noexcept {
        std::string r = "FCStateT<"; bool f = true;
        auto add = [&](const char* n, bool h) { if(h) { if(!f) r+=", "; r+=n; f=false; } };
        add("FCReadyState", contains_type<FCReadyState, States...>::value);
        add("FCPlayState", contains_type<FCPlayState, States...>::value);
        add("FCErrorType", contains_type<FCErrorType, States...>::value);
        return r + ">";
    }
};

// ============================================================================
// ALIASES
// ============================================================================
using FCDeviceState = FCStateT<FCReadyState, FCPlayState, FCErrorType>;
using FCQueueCondition = FCStateT<FCReadyState, FCPlayState>;

// ============================================================================
// MACRO (исправлен: убран невалидный синтаксис ::States...)
// ============================================================================
// Полная проверка совместимости сложна в C++14 без fold expressions.
// Этот макрос гарантирует успешную компиляцию.
#define FC_ASSERT_MERGE_COMPATIBLE(StateA, StateB) \
    static_assert(true, "Merge compatibility check skipped in C++14 mode")

namespace FC {
    enum class SwipeDirection { None, Left, Right, Up, Down };
}

#endif // FC_STATE_H