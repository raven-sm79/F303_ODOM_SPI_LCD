#pragma once
#include <stdint.h>

#define LCD_BLACK 0x0000
#define LCD_WHITE 0xFFFF

/* базовые примитивы */
void lcd_init(void);
void lcd_pixel(uint16_t x, uint16_t y, uint16_t color);

void lcd_fill_screen(uint16_t color);
void lcd_fill_rect(uint16_t x, uint16_t y,
                   uint16_t w, uint16_t h,
                   uint16_t color);

void lcd_draw_hline(uint16_t x, uint16_t y,
                    uint16_t w, uint16_t color);

void lcd_draw_vline(uint16_t x, uint16_t y,
                    uint16_t h, uint16_t color);

void lcd_draw_rect(uint16_t x, uint16_t y,
                   uint16_t w, uint16_t h,
                   uint16_t color);

/* 1bpp bitmap (шрифты, иконки) */
void lcd_draw_mono_1bpp(uint16_t x, uint16_t y,
                        uint16_t w, uint16_t h,
                        const uint8_t *bmp,
                        uint16_t bytes_per_row,
                        uint16_t fg, uint16_t bg);
