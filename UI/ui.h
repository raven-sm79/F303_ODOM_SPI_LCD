#pragma once
#include <stdint.h>

/* Данные для полной перерисовки */
typedef struct {
    uint8_t  hh;
    uint8_t  mm;

    uint8_t  dd;
    uint8_t  mo;
    uint16_t yyyy;

    uint16_t volt_mv;   // например 14700 = 14.7V
    int16_t  temp_c;    // целые градусы, например -12, 26

    uint32_t odo_main;  // 6 цифр (0..999999)

    uint32_t odo0;      // 5 цифр
    uint32_t odo1;      // 5 цифр
    uint32_t odo2;      // 5 цифр
    // (право-низ при желании добавим позже)
} ui_data_t;

/* Инициализация UI (пока пустая, на будущее) */
void ui_init(void);

/* Статическая часть: фон, рамки, разделители */
void ui_draw_static(void);

/* Полная перерисовка динамики (поверх статики) */
void ui_draw_all(const ui_data_t *d);

/* Отдельные обновления (если захочешь “dirty-rect” позже) */
void ui_draw_time(uint8_t hh, uint8_t mm);
void ui_draw_date(uint8_t dd, uint8_t mo, uint16_t yyyy);
void ui_draw_voltage(uint16_t mv);
void ui_draw_temp(int16_t t);
void ui_draw_odo_main(uint32_t val);
void ui_draw_counter0(uint32_t val);
void ui_draw_counter1(uint32_t val);
void ui_draw_counter2(uint32_t val);

/* Тестовый экран (фиксированные значения) */
void ui_draw_static_test(void);
