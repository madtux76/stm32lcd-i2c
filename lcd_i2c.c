#include "main.h"
#include "lcd_i2c.h"
#include "i2c.h"
uint8_t _addr ,_cols ,_rows ,_charsize ,_backlightval,_displayfunction,_displaycontrol,_displaymode;

void send(uint8_t value, uint8_t mode);
void write4bits(uint8_t value);
void expanderWrite(uint8_t _data);
void pulseEnable(uint8_t _data);

// write either command or data
void send(uint8_t value, uint8_t mode) {
	uint8_t highnib=value&0xf0;
	uint8_t lownib=(value<<4)&0xf0;
	write4bits((highnib)|mode);
	write4bits((lownib)|mode);
};

void write4bits(uint8_t value) {
	expanderWrite(value);
	pulseEnable(value);
};

void expanderWrite(uint8_t _data){
	uint8_t temp=0;
	temp =(_data) | _backlightval;
	I2C_Write(_addr,temp);
};

void pulseEnable(uint8_t _data){
	expanderWrite(_data | En);
	HAL_Delay(1);

	expanderWrite(_data & ~En);
	HAL_Delay(5);
};

inline void command(uint8_t value) {
	send(value, 0);
}

inline void write(uint8_t value) {
	send(value, Rs);
}


void Lcd_init(uint8_t lcd_addr, uint8_t lcd_cols, uint8_t lcd_rows, uint8_t charsize)
{
_addr = lcd_addr<<1;
	_cols = lcd_cols;
	_rows = lcd_rows;
	_charsize = charsize;
	_backlightval = LCD_BACKLIGHT;


//
	_displayfunction = LCD_4BITMODE | LCD_1LINE | LCD_5x8DOTS;

	if (_rows > 1) {
		_displayfunction |= LCD_2LINE;
	}

	// for some 1 line displays you can select a 10 pixel high font
	if ((_charsize != 0) && (_rows == 1)) {
		_displayfunction |= LCD_5x10DOTS;
	}

	// SEE PAGE 45/46 FOR INITIALIZATION SPECIFICATION!
	// according to datasheet, we need at least 40ms after power rises above 2.7V
	// before sending commands. Arduino can turn on way befer 4.5V so we'll wait 50
	HAL_Delay(50);

	// Now we pull both RS and R/W low to begin commands
	expanderWrite(_backlightval);	// reset expanderand turn backlight off (Bit 8 =1)
	HAL_Delay(100);

	//put the LCD into 4 bit mode
	// this is according to the hitachi HD44780 datasheet
	// figure 24, pg 46

	// we start in 8bit mode, try to set 4 bit mode
	write4bits(0x03 << 4);
	HAL_Delay(450); // wait min 4.1ms

	// second try
	write4bits(0x03 << 4);
	HAL_Delay(450); // wait min 4.1ms

	// third go!
	write4bits(0x03 << 4);
	HAL_Delay(150);

	// finally, set to 4-bit interface
	write4bits(0x02 << 4);

	// set # lines, font size, etc.
	command(LCD_FUNCTIONSET | _displayfunction);

	// turn the display on with no cursor or blinking default
	_displaycontrol = LCD_DISPLAYON | LCD_CURSOROFF | LCD_BLINKOFF;
	Lcd_display();

	// clear it off
	Lcd_clear();

	// Initialize to default text direction (for roman languages)
	_displaymode = LCD_ENTRYLEFT | LCD_ENTRYSHIFTDECREMENT;

	// set the entry mode
	command(LCD_ENTRYMODESET | _displaymode);

	Lcd_home();
};

void Lcd_clear(void){
	command(LCD_CLEARDISPLAY);// clear display, set cursor position to zero
	HAL_Delay(50);  // this command takes a long time!
};
void Lcd_home(void){
	command(LCD_RETURNHOME);  // set cursor position to zero
	HAL_Delay(50);  // this command takes a long time!
};

// Turn the display on/off (quickly)
void Lcd_noDisplay(void) {
	_displaycontrol &= ~LCD_DISPLAYON;
	command(LCD_DISPLAYCONTROL | _displaycontrol);
};

void Lcd_display(void) {
	_displaycontrol |= LCD_DISPLAYON;
	command(LCD_DISPLAYCONTROL | _displaycontrol);
};

// Turns the underline cursor on/off
void Lcd_noCursor(void) {
	_displaycontrol &= ~LCD_CURSORON;
	command(LCD_DISPLAYCONTROL | _displaycontrol);
};

void Lcd_cursor(void) {
	_displaycontrol |= LCD_CURSORON;
	command(LCD_DISPLAYCONTROL | _displaycontrol);
};

// Turn on and off the blinking cursor
void Lcd_noBlink(void) {
	_displaycontrol &= ~LCD_BLINKON;
	command(LCD_DISPLAYCONTROL | _displaycontrol);
};

void Lcd_blink(void) {
	_displaycontrol |= LCD_BLINKON;
	command(LCD_DISPLAYCONTROL | _displaycontrol);
};
// ---------------------------------------------------------------------------------------------------------------------
void Lcd_setCursor(uint8_t col, uint8_t row)
{
	int row_offsets[] = { 0x00, 0x40, 0x14, 0x54 };
	if (row > _rows) {
		row = _rows-1;    // we count rows starting w/0
	}
	command(LCD_SETDDRAMADDR | (col + row_offsets[row]));
};

void Lcd_scrollDisplayLeft(void) {
	command(LCD_CURSORSHIFT | LCD_DISPLAYMOVE | LCD_MOVELEFT);
};

void Lcd_scrollDisplayRight(void) {
	command(LCD_CURSORSHIFT | LCD_DISPLAYMOVE | LCD_MOVERIGHT);
};

// This is for text that flows Left to Right
void Lcd_leftToRight(void) {
	_displaymode |= LCD_ENTRYLEFT;
	command(LCD_ENTRYMODESET | _displaymode);
};

// This is for text that flows Right to Left
void Lcd_rightToLeft(void) {
	_displaymode &= ~LCD_ENTRYLEFT;
	command(LCD_ENTRYMODESET | _displaymode);
};

// This will 'right justify' text from the cursor
void Lcd_autoscroll(void) {
	_displaymode |= LCD_ENTRYSHIFTINCREMENT;
	command(LCD_ENTRYMODESET | _displaymode);
};

// This will 'left justify' text from the cursor
void Lcd_noAutoscroll(void) {
	_displaymode &= ~LCD_ENTRYSHIFTINCREMENT;
	command(LCD_ENTRYMODESET | _displaymode);
};

void Lcd_nobacklight(void) {
	_backlightval=LCD_NOBACKLIGHT;
	expanderWrite(0);
};

void Lcd_backlight(void) {
	_backlightval=LCD_BACKLIGHT;
	expanderWrite(0);
};

void Lcd_createChar(uint8_t location, uint8_t charmap[]) {
	location &= 0x7; // we only have 8 locations 0-7
	command(LCD_SETCGRAMADDR | (location << 3));
	for (int i=0; i<8; i++) {
		write(charmap[i]);
	}
};
void Lcd_load_custom_character(uint8_t char_num, uint8_t *rows){
	Lcd_createChar(char_num, rows);
};

void Lcd_print(char *buffer,uint8_t lat){
uint8_t i;
for (i=0;i<lat;i++)
    write(buffer[i]);
}

