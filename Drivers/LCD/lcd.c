#include "lcd.h"
#include "st7789.h"

/* ---------- базовые ---------- */

void lcd_init(void)
{
    st7789_init();
}

void lcd_pixel(uint16_t x, uint16_t y, uint16_t color)
{
    st7789_set_window(x, y, x, y);
    st7789_write_pixel(color);
}

void lcd_fill_screen(uint16_t color)
{
    st7789_set_window(0, 0, 239, 319);
    for (uint32_t i = 0; i < 240u * 320u; i++)
        st7789_write_pixel(color);
}

void lcd_fill_rect(uint16_t x, uint16_t y,
                   uint16_t w, uint16_t h,
                   uint16_t color)
{
    st7789_set_window(x, y, x + w - 1, y + h - 1);
    for (uint32_t i = 0; i < (uint32_t)w * h; i++)
        st7789_write_pixel(color);
}

/* ---------- линии и прямоугольники ---------- */

void lcd_draw_hline(uint16_t x, uint16_t y,
                    uint16_t w, uint16_t color)
{
    lcd_fill_rect(x, y, w, 1, color);
}

void lcd_draw_vline(uint16_t x, uint16_t y,
                    uint16_t h, uint16_t color)
{
    lcd_fill_rect(x, y, 1, h, color);
}

void lcd_draw_rect(uint16_t x, uint16_t y,
                   uint16_t w, uint16_t h,
                   uint16_t color)
{
    lcd_draw_hline(x, y, w, color);
    lcd_draw_hline(x, y + h - 1, w, color);
    lcd_draw_vline(x, y, h, color);
    lcd_draw_vline(x + w - 1, y, h, color);
}

/* ---------- монохром 1bpp ---------- */

void lcd_draw_mono_1bpp(uint16_t x, uint16_t y,
                        uint16_t w, uint16_t h,
                        const uint8_t *bmp,
                        uint16_t bpr,
                        uint16_t fg, uint16_t bg)
{
    for (uint16_t yy = 0; yy < h; yy++) {
        const uint8_t *row = bmp + yy * bpr;
        for (uint16_t xx = 0; xx < w; xx++) {
            uint8_t byte = row[xx >> 3];
            uint8_t bit  = 7 - (xx & 7);
            uint16_t col = (byte & (1 << bit)) ? fg : bg;
            lcd_pixel(x + xx, y + yy, col);
        }
    }
}
