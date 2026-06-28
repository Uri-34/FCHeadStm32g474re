// 1. Объявляете типы (обычно в .h файле)
enum class AppEvent { CmdStart, CmdPause, CmdResume, CmdStop, Fault };

// 2. Создаёте один объект-контроллер
FCStateController<FCPlayState, FCReadyState, FCErrorType, AppEvent> ctrl;

// 3. Настраиваете (обычно в конструкторе)
void setupController() {
    // Регистрируем entry/exit для состояний
    ctrl.addState(FCPlayState::Start, 
        [](FCPlayState, FCPlayState, void* ctx) { 
            static_cast<FCStateController<...>*>(ctx)->set(FCPlayState::Start); 
            // ctrl._motors.enable(); 
        },
        [](FCPlayState, FCPlayState, void* ctx) { 
            // ctrl._motors.disable(); 
        });

    // Добавляем переходы. Guard проверяет флаги из того же объекта!
    ctrl.addTransition(FCPlayState::Stop, AppEvent::CmdStart, FCPlayState::Start,
        [](FCPlayState, AppEvent, void* ctx) {
            auto* c = static_cast<FCStateController<...>*>(ctx);
            return c->is(FCReadyState::Ready); // ✅ Проверяем флаг из того же объекта
        });

    ctrl.addTransition(FCPlayState::Start, AppEvent::CmdPause, FCPlayState::Pause);
    ctrl.addTransition(FCPlayState::Pause,  AppEvent::CmdResume, FCPlayState::Start);
    ctrl.addTransition(FCPlayState::Start,  AppEvent::CmdStop,   FCPlayState::Stop);
    ctrl.addTransition(FCPlayState::Start,  AppEvent::Fault,     FCPlayState::Stop);

    // Запуск
    ctrl.start(FCPlayState::Stop);
}

// 4. Использование в любом потоке/ISR (безопасно через EventQueue)
void onButtonPressed() {
    // В реальном Mbed проекте:
    // _queue.call([]() { ctrl.process(AppEvent::CmdStart); });
    ctrl.process(AppEvent::CmdStart);
}

// 5. Чтение состояния в цикле или UI
void loop() {
    if (ctrl.is(FCPlayState::Start) && ctrl.get<FCErrorType>() == FCErrorType::None) {
        // Работаем
    }
}
