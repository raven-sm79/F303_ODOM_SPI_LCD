#include "ui.h"

#include <stdio.h>
#include <string.h>

#include "../Drivers/LCD/lcd.h"
#include "../Drivers/GFX/gfx.h"

/* Fonts (данные) */
#include "../Drivers/Fonts/font12_symbols.h"
#include "../Drivers/Fonts/font27_digits.h"
#include "../Drivers/Fonts/font36_digits.h"
#include "../Drivers/Fonts/font75_digits.h"
#include "../Drivers/Fonts/font75_punct.h"

/* ===== UI COLORS (фиксируем) ===== */
#define UI_FG LCD_WHITE
#define UI_BG LCD_BLACK

/* ===== Макет экрана 240x320 (книжная) ===== */

/* Верхняя строка: напряжение/температура */
#define X_VOLT    20
#define X_TEMP   170
#define Y_TOPTXT  50

/* Часы крупные */
#define X_TIME    10
#define Y_TIME    83

/* Дата */
#define X_DATE    80
#define Y_DATE   165

/* Основной пробег (сверху) */
#define X_ODO_MAIN_CENTER 120
#define Y_ODO_MAIN         10

/* Рамка блоков счетчиков */
#define BOX_X      5
#define BOX_Y    200
#define BOX_W    230
#define BOX_H    115
#define BOX_VX   120
#define BOX_HY   (BOX_Y + BOX_H/2)

/* Позиции счетчиков */
#define X_CNT_L_RIGHT   115   /* правый край левого блока */
#define X_CNT_R_RIGHT   230   /* правый край правого блока */
#define Y_CNT_0         215
#define Y_CNT_1         272

/* ===== Внутренние helper’ы ===== */

static inline void draw_mono(uint16_t x, uint16_t y,
                             uint16_t w, uint16_t h, uint16_t bpr,
                             const uint8_t *bmp,
                             uint16_t fg, uint16_t bg)
{
    gfx_draw_mono(x, y, w, h, bpr, bmp, fg, bg);
}

/* ---------- font12 symbols ---------- */

static int font12_find(char ch)
{
    for (int i = 0; font12_symbols_map[i] != 0; i++) {
        if (font12_symbols_map[i] == ch) return i;
    }
    return -1;
}

static void ui_draw_char12(uint16_t x, uint16_t y, char ch, uint16_t fg, uint16_t bg)
{
    int idx = font12_find(ch);
    if (idx < 0) return;

    int8_t yo = font12_y_offset[idx];

    lcd_draw_mono_1bpp(
        x,
        y - yo,   // ← ВОТ ОНО
        FONT12_SYMBOLS_W,
        FONT12_SYMBOLS_H,
        &font12_symbols[idx][0],
        FONT12_SYMBOLS_BPR,
        fg, bg
    );
}

static void ui_draw_string12(uint16_t x, uint16_t y, const char *s, uint16_t fg, uint16_t bg)
{
    uint16_t cx = x;
    while (*s) {
        ui_draw_char12(cx, y, *s, fg, bg);
        cx += (FONT12_SYMBOLS_W + 1); // шаг между символами
        s++;
    }
}

/* ---------- font75 time digits + punct ':' ---------- */

static void ui_draw_digit75(uint16_t x, uint16_t y, uint8_t d, uint16_t fg, uint16_t bg)
{
    if (d > 9) return;
    draw_mono(x, y, FONT75_DIGITS_W, FONT75_DIGITS_H, FONT75_DIGITS_BPR,
              &font75_digits[d][0], fg, bg);
}

static void ui_draw_colon75(uint16_t x, uint16_t y, uint16_t fg, uint16_t bg)
{
    /* ':' у тебя один символ => индекс 0 */
    (void)font75_punct_map; // на будущее, если расширишь
    draw_mono(x, y, FONT75_PUNCT_W, FONT75_PUNCT_H, FONT75_PUNCT_BPR,
              &font75_punct[0][0], fg, bg);
}

/* ---------- font36 main odo (6 digits) ---------- */

static void ui_draw_digit36(uint16_t x, uint16_t y, uint8_t d, uint16_t fg, uint16_t bg)
{
    if (d > 9) return;
    draw_mono(x, y, FONT36_DIGITS_W, FONT36_DIGITS_H, FONT36_DIGITS_BPR,
              &font36_digits[d][0], fg, bg);
}

static void ui_draw_number6_font36_center(uint16_t cx, uint16_t y, uint32_t val, uint16_t fg, uint16_t bg)
{
    char s[7];
    s[6] = 0;
    for (int i = 5; i >= 0; i--) { s[i] = (val % 10) + '0'; val /= 10; }

    uint16_t dx = FONT36_DIGITS_W + 1;
    uint16_t total_w = 6 * dx - 1;
    uint16_t x = (cx > total_w/2) ? (cx - total_w/2) : 0;

    for (int i = 0; i < 6; i++) {
        ui_draw_digit36(x + i*dx, y, (uint8_t)(s[i] - '0'), fg, bg);
    }
}

/* ---------- font27 counters (5 digits) ---------- */

static void ui_draw_digit27(uint16_t x, uint16_t y, uint8_t d, uint16_t fg, uint16_t bg)
{
    if (d > 9) return;
    draw_mono(x, y, FONT27_DIGITS_W, FONT27_DIGITS_H, FONT27_DIGITS_BPR,
              &font27_digits[d][0], fg, bg);
}

static void ui_draw_number5_font27_right(uint16_t rx, uint16_t y, uint32_t val, uint16_t fg, uint16_t bg)
{
    char s[6];
    s[5] = 0;
    for (int i = 4; i >= 0; i--) { s[i] = (val % 10) + '0'; val /= 10; }

    uint16_t dx = FONT27_DIGITS_W + 1;
    uint16_t total_w = 5 * dx - 1;
    uint16_t x = (rx > total_w) ? (rx - total_w) : 0;

    for (int i = 0; i < 5; i++) {
        ui_draw_digit27(x + i*dx, y, (uint8_t)(s[i] - '0'), fg, bg);
    }
}

/* ===== Public API ===== */

void ui_init(void)
{
    /* пока пусто */
}

void ui_draw_static(void)
{
    lcd_fill_screen(UI_BG);

    /* Рамка блока счётчиков */
    lcd_draw_rect(BOX_X, BOX_Y, BOX_W, BOX_H, UI_FG);

    /* Разделители */
    lcd_draw_vline(BOX_VX, BOX_Y, BOX_H, UI_FG);
    lcd_draw_hline(BOX_X, BOX_HY, BOX_W, UI_FG);

    /* (позже) иконки в квадрантах */
}

/* ----- Дата/темп/вольты ----- */

void ui_draw_date(uint8_t dd, uint8_t mo, uint16_t yyyy)
{
    char buf[16];
    snprintf(buf, sizeof(buf), "%02u.%02u.%04u", dd, mo, (unsigned)yyyy);

    /* очистка области даты */
    lcd_fill_rect(40, Y_DATE-2, 200, FONT12_SYMBOLS_H + 4, UI_BG);

    /* центрируем примерно вокруг X_DATE */
    ui_draw_string12(X_DATE - 4*(FONT12_SYMBOLS_W + 1), Y_DATE, buf, UI_FG, UI_BG);
}

void ui_draw_voltage(uint16_t mv)
{
    /* 14700 -> "14.7V" */
    uint16_t v = mv / 1000;
    uint16_t d1 = (mv % 1000) / 100; // десятые

    char buf[8];
    snprintf(buf, sizeof(buf), "%u.%uV", (unsigned)v, (unsigned)d1);

    /* очистка */
    lcd_fill_rect(0, Y_TOPTXT-5, 120, FONT12_SYMBOLS_H + 10, UI_BG);

    ui_draw_string12(X_VOLT, Y_TOPTXT, buf, UI_FG, UI_BG);
}

void ui_draw_temp(int16_t t)
{
    char num[8];
    int neg = (t < 0);
    if (neg) t = -t;

    if (neg) snprintf(num, sizeof(num), "-%d", (int)t);
    else     snprintf(num, sizeof(num), "%d",  (int)t);

    /* очистка */
    lcd_fill_rect(120, Y_TOPTXT-5, 120, FONT12_SYMBOLS_H + 10, UI_BG);

    /* число */
    ui_draw_string12(X_TEMP, Y_TOPTXT, num, UI_FG, UI_BG);

    /* градус + C по длине числа */
    uint16_t len = (uint16_t)strlen(num);
    uint16_t xdeg = X_TEMP + len * (FONT12_SYMBOLS_W + 1);

    ui_draw_char12(xdeg, Y_TOPTXT, '\xB0', UI_FG, UI_BG);
    ui_draw_char12(xdeg + (FONT12_SYMBOLS_W + 1), Y_TOPTXT, 'C', UI_FG, UI_BG);
}

/* ----- Время ----- */

void ui_draw_time(uint8_t hh, uint8_t mm)
{
    uint16_t x = X_TIME;
    uint16_t y = Y_TIME;

    /* очистим область часов целиком (чтобы не было “хвостов”) */
    lcd_fill_rect(0, Y_TIME-2, 240, FONT75_DIGITS_H + 4, UI_BG);

    uint16_t dx = FONT75_DIGITS_W + 2;

    ui_draw_digit75(x + dx*0, y, hh/10, UI_FG, UI_BG);
    ui_draw_digit75(x + dx*1, y, hh%10, UI_FG, UI_BG);

    /* ':' (позиция подобрана под твой макет; если надо — двигай 113/ y-10) */
    ui_draw_colon75(113, (uint16_t)(y - 10), UI_FG, UI_BG);

    ui_draw_digit75(x + dx*3, y, mm/10, UI_FG, UI_BG);
    ui_draw_digit75(x + dx*4, y, mm%10, UI_FG, UI_BG);
}

/* ----- Одометр основной ----- */

void ui_draw_odo_main(uint32_t val)
{
    if (val > 999999) val %= 1000000;

    /* очистка области */
    lcd_fill_rect(0, Y_ODO_MAIN, 240, FONT36_DIGITS_H + 2, UI_BG);

    ui_draw_number6_font36_center(X_ODO_MAIN_CENTER, Y_ODO_MAIN, val, UI_FG, UI_BG);
}

/* ----- Счётчики ----- */

void ui_draw_counter0(uint32_t val)
{
    if (val > 99999) val %= 100000;
    lcd_fill_rect(5, 205, 115, 50, UI_BG);
    ui_draw_number5_font27_right(X_CNT_L_RIGHT, Y_CNT_0, val, UI_FG, UI_BG);
}

void ui_draw_counter1(uint32_t val)
{
    if (val > 99999) val %= 100000;
    lcd_fill_rect(5, 262, 115, 50, UI_BG);
    ui_draw_number5_font27_right(X_CNT_L_RIGHT, Y_CNT_1, val, UI_FG, UI_BG);
}

void ui_draw_counter2(uint32_t val)
{
    if (val > 99999) val %= 100000;
    lcd_fill_rect(120, 205, 115, 50, UI_BG);
    ui_draw_number5_font27_right(X_CNT_R_RIGHT, Y_CNT_0, val, UI_FG, UI_BG);
}

/* ----- Полная перерисовка ----- */

void ui_draw_all(const ui_data_t *d)
{
    ui_draw_time(d->hh, d->mm);
    ui_draw_date(d->dd, d->mo, d->yyyy);
    ui_draw_voltage(d->volt_mv);
    ui_draw_temp(d->temp_c);

    ui_draw_odo_main(d->odo_main);

    ui_draw_counter0(d->odo0);
    ui_draw_counter1(d->odo1);
    ui_draw_counter2(d->odo2);
}

/* ----- Тест ----- */

void ui_draw_static_test(void)
{
    ui_draw_static();

    ui_draw_voltage(14700);
    ui_draw_temp(-12);

    ui_draw_time(22, 10);
    ui_draw_date(22, 3, 2026);

    ui_draw_odo_main(123456);

    ui_draw_counter0(88);
    ui_draw_counter1(88);
    ui_draw_counter2(5488);
}
