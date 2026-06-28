#include <cstdio>
#include <cstdint>

#include "FCState.h"

// Простой тестовый фреймворк
static int g_passes = 0;
static int g_failures = 0;

#define TEST_ASSERT(condition, message) \
    do { \
        if (!(condition)) { \
            printf("✗ FAIL: %s (строка %d)\n", message, __LINE__); \
            g_failures++; \
        } else { \
            printf("✓ PASS: %s\n", message); \
            g_passes++; \
        } \
    } while(0)

void test_size() {
    printf("\n=== Тест 1: Размер класса ===\n");
    TEST_ASSERT(sizeof(FCCondition) == 1, "Размер FCCondition должен быть 1 байт");
    TEST_ASSERT(sizeof(FCState) == 1, "Размер FCState должен быть 1 байт");
}

void test_default_initialization() {
    printf("\n=== Тест 2: Инициализация по умолчанию ===\n");
    FCCondition state;
    
    TEST_ASSERT(state.getRaw() == 0, "Начальное состояние должно быть 0x00");
    TEST_ASSERT(!state.is(FCState::Ready), "Ready должен быть false");
    TEST_ASSERT(!state.is(FCState::Started), "Started должен быть false");
    TEST_ASSERT(!state.is(FCState::Paused), "Paused должен быть false");
    TEST_ASSERT(!state.is(FCState::Changed), "Changed должен быть false");
    TEST_ASSERT(!state.is(FCState::ErrorAny), "ErrorAny должен быть false");
    TEST_ASSERT(state.get(FCState::ErrorAny) == 0, "Код ошибки должен быть 0");
}

void test_ready_field() {
    printf("\n=== Тест 3: Поле Ready ===\n");
    FCCondition state;
    
    state.set(FCState::Ready);
    TEST_ASSERT(state.is(FCState::Ready), "Ready должен быть true после set");
    TEST_ASSERT(state.get(FCState::Ready) == 1, "get(Ready) должен вернуть 1");
    TEST_ASSERT(state.getRaw() == 0x01, "Raw байт должен быть 0x01");
    
    state.set(FCState::NotReady);
    TEST_ASSERT(!state.is(FCState::Ready), "Ready должен быть false после NotReady");
    TEST_ASSERT(state.get(FCState::Ready) == 0, "get(Ready) должен вернуть 0");
    TEST_ASSERT(state.getRaw() == 0x00, "Raw байт должен быть 0x00");
}

void test_started_field() {
    printf("\n=== Тест 4: Поле Started ===\n");
    FCCondition state;
    
    state.set(FCState::Started);
    TEST_ASSERT(state.is(FCState::Started), "Started должен быть true");
    TEST_ASSERT(state.get(FCState::Started) == 1, "get(Started) должен вернуть 1");
    TEST_ASSERT(state.getRaw() == 0x02, "Raw байт должен быть 0x02");
    
    state.set(FCState::Stopped);
    TEST_ASSERT(!state.is(FCState::Started), "Started должен быть false после Stopped");
    TEST_ASSERT(state.getRaw() == 0x00, "Raw байт должен быть 0x00");
}

void test_paused_field() {
    printf("\n=== Тест 5: Поле Paused ===\n");
    FCCondition state;
    
    state.set(FCState::Paused);
    TEST_ASSERT(state.is(FCState::Paused), "Paused должен быть true");
    TEST_ASSERT(state.get(FCState::Paused) == 1, "get(Paused) должен вернуть 1");
    TEST_ASSERT(state.getRaw() == 0x40, "Raw байт должен быть 0x40");
    
    state.set(FCState::Resumed);
    TEST_ASSERT(!state.is(FCState::Paused), "Paused должен быть false после Resumed");
    TEST_ASSERT(state.getRaw() == 0x00, "Raw байт должен быть 0x00");
}

void test_changed_field() {
    printf("\n=== Тест 6: Поле Changed ===\n");
    FCCondition state;
    
    state.set(FCState::Changed);
    TEST_ASSERT(state.is(FCState::Changed), "Changed должен быть true");
    TEST_ASSERT(state.get(FCState::Changed) == 1, "get(Changed) должен вернуть 1");
    TEST_ASSERT(state.getRaw() == 0x80, "Raw байт должен быть 0x80");
    
    state.set(FCState::NotChanged);
    TEST_ASSERT(!state.is(FCState::Changed), "Changed должен быть false после NotChanged");
    TEST_ASSERT(state.getRaw() == 0x00, "Raw байт должен быть 0x00");
}

void test_all_errors() {
    printf("\n=== Тест 7: Все коды ошибок ===\n");
    FCCondition state;
    
    // Тестируем каждую ошибку
    state.set(FCState::ErrorNone);
    TEST_ASSERT(state.get(FCState::ErrorAny) == 0, "ErrorNone: код должен быть 0");
    TEST_ASSERT(!state.is(FCState::ErrorAny), "ErrorNone: ErrorAny должен быть false");
    
    state.set(FCState::ErrorOpen);
    TEST_ASSERT(state.get(FCState::ErrorAny) == 1, "ErrorOpen: код должен быть 1");
    TEST_ASSERT(state.is(FCState::ErrorOpen), "ErrorOpen: is должен вернуть true");
    TEST_ASSERT(!state.is(FCState::ErrorClose), "ErrorOpen: ErrorClose должен быть false");
    
    state.set(FCState::ErrorClose);
    TEST_ASSERT(state.get(FCState::ErrorAny) == 2, "ErrorClose: код должен быть 2");
    TEST_ASSERT(state.is(FCState::ErrorClose), "ErrorClose: is должен вернуть true");
    
    state.set(FCState::ErrorConnection);
    TEST_ASSERT(state.get(FCState::ErrorAny) == 3, "ErrorConnection: код должен быть 3");
    
    state.set(FCState::ErrorDisconnection);
    TEST_ASSERT(state.get(FCState::ErrorAny) == 4, "ErrorDisconnection: код должен быть 4");
    
    state.set(FCState::ErrorCreate);
    TEST_ASSERT(state.get(FCState::ErrorAny) == 5, "ErrorCreate: код должен быть 5");
    
    state.set(FCState::ErrorDestroy);
    TEST_ASSERT(state.get(FCState::ErrorAny) == 6, "ErrorDestroy: код должен быть 6");
    
    state.set(FCState::ErrorRead);
    TEST_ASSERT(state.get(FCState::ErrorAny) == 7, "ErrorRead: код должен быть 7");
    
    state.set(FCState::ErrorWrite);
    TEST_ASSERT(state.get(FCState::ErrorAny) == 8, "ErrorWrite: код должен быть 8");
    
    state.set(FCState::ErrorOutOfRange);
    TEST_ASSERT(state.get(FCState::ErrorAny) == 9, "ErrorOutOfRange: код должен быть 9");
    
    state.set(FCState::ErrorConfiguration);
    TEST_ASSERT(state.get(FCState::ErrorAny) == 10, "ErrorConfiguration: код должен быть 10");
    
    state.set(FCState::ErrorParse);
    TEST_ASSERT(state.get(FCState::ErrorAny) == 11, "ErrorParse: код должен быть 11");
    
    state.set(FCState::ErrorTimeout);
    TEST_ASSERT(state.get(FCState::ErrorAny) == 12, "ErrorTimeout: код должен быть 12");
    TEST_ASSERT(state.is(FCState::ErrorTimeout), "ErrorTimeout: is должен вернуть true");
    
    state.set(FCState::ErrorEmergencyStoped);
    TEST_ASSERT(state.get(FCState::ErrorAny) == 13, "ErrorEmergencyStoped: код должен быть 13");
    
    state.set(FCState::ErrorMotion);
    TEST_ASSERT(state.get(FCState::ErrorAny) == 14, "ErrorMotion: код должен быть 14");
    
    // Сброс ошибки
    state.set(FCState::ErrorNone);
    TEST_ASSERT(state.get(FCState::ErrorAny) == 0, "После ErrorNone код должен быть 0");
    TEST_ASSERT(!state.is(FCState::ErrorAny), "После ErrorNone ErrorAny должен быть false");
}

void test_field_isolation() {
    printf("\n=== Тест 8: Изоляция полей ===\n");
    FCCondition state;
    
    // Устанавливаем все флаги
    state.set(FCState::Ready);
    state.set(FCState::Started);
    state.set(FCState::Paused);
    state.set(FCState::Changed);
    
    uint8_t before = state.getRaw();
    TEST_ASSERT(before == 0xC3, "Все флаги установлены: raw должен быть 0xC3");
    
    // Устанавливаем ошибку
    state.set(FCState::ErrorTimeout);
    
    uint8_t after = state.getRaw();
    
    // Проверяем, что биты 0, 1, 6, 7 не изменились
    TEST_ASSERT((before & 0xC3) == (after & 0xC3), 
                "Установка ошибки не должна менять другие флаги");
    TEST_ASSERT(state.is(FCState::Ready), "Ready не должен измениться");
    TEST_ASSERT(state.is(FCState::Started), "Started не должен измениться");
    TEST_ASSERT(state.is(FCState::Paused), "Paused не должен измениться");
    TEST_ASSERT(state.is(FCState::Changed), "Changed не должен измениться");
    TEST_ASSERT(state.get(FCState::ErrorAny) == 12, "Код ошибки должен быть 12");
    
    // Меняем флаги, проверяем что ошибка не меняется
    state.set(FCState::NotReady);
    state.set(FCState::Stopped);
    state.set(FCState::Resumed);
    state.set(FCState::NotChanged);
    
    TEST_ASSERT(state.get(FCState::ErrorAny) == 12, 
                "Изменение флагов не должно менять код ошибки");
    TEST_ASSERT(state.is(FCState::ErrorTimeout), "Ошибка Timeout должна остаться");
}

void test_combined_flags() {
    printf("\n=== Тест 9: Комбинации флагов ===\n");
    FCCondition state;
    
    // Устанавливаем несколько флагов
    state.set(FCState::Ready);
    state.set(FCState::Started);
    state.set(FCState::ErrorTimeout);
    
    TEST_ASSERT(state.is(FCState::Ready), "Ready должен быть true");
    TEST_ASSERT(state.is(FCState::Started), "Started должен быть true");
    TEST_ASSERT(state.is(FCState::ErrorTimeout), "ErrorTimeout должен быть true");
    TEST_ASSERT(!state.is(FCState::Paused), "Paused должен быть false");
    TEST_ASSERT(!state.is(FCState::Changed), "Changed должен быть false");
    
    // Raw байт: Ready(0x01) | Started(0x02) | Error(12 << 2 = 0x30) = 0x33
    TEST_ASSERT(state.getRaw() == 0x33, "Raw байт должен быть 0x33");
}

void test_raw_constructor() {
    printf("\n=== Тест 10: Конструктор с raw значением ===\n");
    
    // Ready + Started + Paused + Changed = 0x01 | 0x02 | 0x40 | 0x80 = 0xC3
    FCCondition state1(0xC3);
    TEST_ASSERT(state1.is(FCState::Ready), "Ready должен быть true");
    TEST_ASSERT(state1.is(FCState::Started), "Started должен быть true");
    TEST_ASSERT(state1.is(FCState::Paused), "Paused должен быть true");
    TEST_ASSERT(state1.is(FCState::Changed), "Changed должен быть true");
    
    // Error 5 (0x14) + Ready (0x01) = 0x15
    FCCondition state2(0x15);
    TEST_ASSERT(state2.is(FCState::Ready), "Ready должен быть true");
    TEST_ASSERT(state2.get(FCState::ErrorAny) == 5, "Код ошибки должен быть 5");
    TEST_ASSERT(state2.is(FCState::ErrorCreate), "Должна быть ошибка Create");
}

void test_setRaw() {
    printf("\n=== Тест 11: Метод setRaw ===\n");
    FCCondition state;
    
    state.setRaw(0xC3);
    TEST_ASSERT(state.getRaw() == 0xC3, "setRaw должен установить значение");
    TEST_ASSERT(state.is(FCState::Ready), "Ready должен быть true");
    TEST_ASSERT(state.is(FCState::Started), "Started должен быть true");
    TEST_ASSERT(state.is(FCState::Paused), "Paused должен быть true");
    TEST_ASSERT(state.is(FCState::Changed), "Changed должен быть true");
}

void test_clear() {
    printf("\n=== Тест 12: Метод clear ===\n");
    FCCondition state;
    
    state.set(FCState::Ready);
    state.set(FCState::Started);
    state.set(FCState::ErrorTimeout);
    state.set(FCState::Paused);
    state.set(FCState::Changed);
    
    TEST_ASSERT(state.getRaw() != 0, "Состояние должно быть не нулевым");
    
    state.clear();
    
    TEST_ASSERT(state.getRaw() == 0, "После clear() всё должно быть 0");
    TEST_ASSERT(!state.is(FCState::Ready), "Ready должен быть false");
    TEST_ASSERT(!state.is(FCState::Started), "Started должен быть false");
    TEST_ASSERT(!state.is(FCState::ErrorAny), "Ошибка должна быть сброшена");
    TEST_ASSERT(!state.is(FCState::Paused), "Paused должен быть false");
    TEST_ASSERT(!state.is(FCState::Changed), "Changed должен быть false");
}

void test_error_any_special_case() {
    printf("\n=== Тест 13: Специальный случай ErrorAny ===\n");
    FCCondition state;
    
    // ErrorAny должен вернуть true для любой ошибки != 0
    state.set(FCState::ErrorOpen);
    TEST_ASSERT(state.is(FCState::ErrorAny), "ErrorAny должен вернуть true для ErrorOpen");
    
    state.set(FCState::ErrorTimeout);
    TEST_ASSERT(state.is(FCState::ErrorAny), "ErrorAny должен вернуть true для ErrorTimeout");
    
    state.set(FCState::ErrorNone);
    TEST_ASSERT(!state.is(FCState::ErrorAny), "ErrorAny должен вернуть false для ErrorNone");
    
    // is(ErrorAny) должен вернуть false, даже если код ошибки не равен 15
    state.set(FCState::ErrorMotion);
    TEST_ASSERT(!state.is(FCState::ErrorAny), 
                "is(ErrorAny) должен вернуть false, когда код != 15");
    TEST_ASSERT(state.get(FCState::ErrorAny) == 14, 
                "get(ErrorAny) должен вернуть текущий код ошибки");
}

int main() {
    printf("╔═══════════════════════════════════════════════════════════╗\n");
    printf("║       Тестирование класса FCCondition и FCState          ║\n");
    printf("╚═══════════════════════════════════════════════════════════╝\n");
    
    test_size();
    test_default_initialization();
    test_ready_field();
    test_started_field();
    test_paused_field();
    test_changed_field();
    test_all_errors();
    test_field_isolation();
    test_combined_flags();
    test_raw_constructor();
    test_setRaw();
    test_clear();
    test_error_any_special_case();
    
    printf("\n╔═══════════════════════════════════════════════════════════╗\n");
    printf("║                    Результаты тестирования                ║\n");
    printf("╠═══════════════════════════════════════════════════════════╣\n");
    printf("║ Пройдено: %-52d ║\n", g_passes);
    printf("║ Провалено: %-51d ║\n", g_failures);
    printf("╚═══════════════════════════════════════════════════════════╝\n");
    
    if (g_failures == 0) {
        printf("\n✓✓✓ Все тесты пройдены успешно! ✓✓✓\n\n");
        return 0;
    } else {
        printf("\n✗✗✗ Некоторые тесты провалены! ✗✗✗\n\n");
        return 1;
    }
}
