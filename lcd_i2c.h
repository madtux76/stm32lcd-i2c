#ifndef LCD_I2C_H_INCLUDED
#define LCD_I2C_H_INCLUDED
// команды дисплея
#define LCD_CLEARDISPLAY 0x01
#define LCD_RETURNHOME 0x02
#define LCD_ENTRYMODESET 0x04
#define LCD_DISPLAYCONTROL 0x08
#define LCD_CURSORSHIFT 0x10
#define LCD_FUNCTIONSET 0x20
#define LCD_SETCGRAMADDR 0x40
#define LCD_SETDDRAMADDR 0x80

// местоположение вывода на дисплей
#define LCD_ENTRYRIGHT 0x00
#define LCD_ENTRYLEFT 0x02
#define LCD_ENTRYSHIFTINCREMENT 0x01
#define LCD_ENTRYSHIFTDECREMENT 0x00

// переключатели
#define LCD_DISPLAYON 0x04
#define LCD_DISPLAYOFF 0x00
#define LCD_CURSORON 0x02
#define LCD_CURSOROFF 0x00
#define LCD_BLINKON 0x01
#define LCD_BLINKOFF 0x00

// кугсор
#define LCD_DISPLAYMOVE 0x08
#define LCD_CURSORMOVE 0x00
#define LCD_MOVERIGHT 0x04
#define LCD_MOVELEFT 0x00

// настройки
#define LCD_8BITMODE 0x10
#define LCD_4BITMODE 0x00
#define LCD_2LINE 0x08
#define LCD_1LINE 0x00
#define LCD_5x10DOTS 0x04
#define LCD_5x8DOTS 0x00

// подсветка
#define LCD_BACKLIGHT 0x08
#define LCD_NOBACKLIGHT 0x00

#define En 0b00000100  // Разрешения бит
#define Rw 0b00000010  // Чтение запись бит
#define Rs 0b00000001  // Регист выбора бит
void Lcd_init(uint8_t lcd_addr, uint8_t lcd_cols, uint8_t lcd_rows, uint8_t charsize);
void Lcd_clear(void);
void Lcd_home(void);
void Lcd_blink(void);
void Lcd_noBlink(void);
void Lcd_display(void);
void Lcd_noDisplay(void);
void Lcd_cursor(void);
void Lcd_noCursor(void);
void Lcd_display(void);
void Lcd_backlight(void);
void Lcd_nobacklight(void);
void command(uint8_t value);
void write(uint8_t value);
void Lcd_setCursor(uint8_t col, uint8_t row);
void Lcd_scrollDisplayLeft(void);
void Lcd_scrollDisplayRight(void);
void Lcd_leftToRight(void);
void Lcd_rightToLeft(void);
void Lcd_autoscroll(void);
void Lcd_noAutoscroll(void);
void Lcd_createChar(uint8_t location, uint8_t charmap[]);
void Lcd_load_custom_character(uint8_t char_num, uint8_t *rows);
void Lcd_print(char *buffer,uint8_t lat);
#endif // LCD_I2C_H_INCLUDED
