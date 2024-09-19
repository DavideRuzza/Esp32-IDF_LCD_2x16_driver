#include <stdint.h>

#ifndef _LCD_DRIVER_H
#define _LCD_DRIVER_H

// command list
// #define CMD_CLEAR_DISPLAY 0x01
// #define CMD_RETURN_HOME 0x02
// #define CMD_ENTRY_MODE_SET 0x04
// #define CMD_DISPLAY_ON_OFF 0x08
// #define CMD_CURS_DISP_SHIFT 0x10
// #define CMD_FUNCTION_SET 0x20
// #define CMD_CGRAM_ADD_SET 0x40
// #define CMD_DDRAM_ADD_SET 0x80




#define CMD_CLEARDISPLAY 0x01
#define CMD_RETURNHOME 0x02
#define CMD_ENTRYMODESET 0x04
#define CMD_DISPLAYCONTROL 0x08
#define CMD_CURSORSHIFT 0x10
#define CMD_FUNCTIONSET 0x20
#define CMD_SETCGRAMADDR 0x40
#define CMD_SETDDRAMADDR 0x80

// flags for display entry mode
#define LCD_ENTRYRIGHT 0x00
#define LCD_ENTRYLEFT 0x02
#define LCD_ENTRYSHIFTINCREMENT 0x01
#define LCD_ENTRYSHIFTDECREMENT 0x00

// flags for display on/off control
#define LCD_DISPLAYON 0x04
#define LCD_DISPLAYOFF 0x00
#define LCD_CURSORON 0x02
#define LCD_CURSOROFF 0x00
#define LCD_BLINKON 0x01
#define LCD_BLINKOFF 0x00

// flags for display/cursor shift
#define LCD_DISPLAYMOVE 0x08
#define LCD_CURSORMOVE 0x00
#define LCD_MOVERIGHT 0x04
#define LCD_MOVELEFT 0x00

// flags for function set
#define LCD_8BITMODE 0x10
#define LCD_4BITMODE 0x00
#define LCD_2LINE 0x08
#define LCD_1LINE 0x00
#define LCD_5x10DOTS 0x04
#define LCD_5x8DOTS 0x00

typedef struct{
    uint8_t DL; // data length
    uint8_t N;  // number of lines
    uint8_t F;  // font type
    
    uint8_t ID; // increment decrement
    uint8_t S;  // accompany display shift

    uint8_t D;  // display on/off
    uint8_t C;  // cursor on/off
    uint8_t B;  // blink on/off

    uint8_t SC; // shift or cursore move
    uint8_t RL; // move rx/lx


} lcd_setting_t;

typedef struct {
    int rs; // register select: 0 command, 1 data
    int en; // enable 
    int d4; 
    int d5;
    int d6;
    int d7;
    lcd_setting_t setting;
} lcd_t;


void lcd_init(lcd_t *lcd, int rs, int en, int d4, int d5, int d6, int d7);
// void init_lcd(lcd_t *lcd, int rs, int en, int d4, int d5, int d6, int d7);
void pulse_en(lcd_t *lcd);
void send_cmd(lcd_t *lcd, uint8_t instr);
void send_data(lcd_t *lcd, uint8_t data);
void set_cmd_H(lcd_t *lcd, uint8_t word);
void set_cmd_L(lcd_t *lcd, uint8_t word);
void send_H(lcd_t *lcd, uint8_t word);
void send_L(lcd_t *lcd, uint8_t word);

void lcd_clear(lcd_t *lcd);
void lcd_home(lcd_t *lcd);
void lcd_write(lcd_t *lcd, char c);
void lcd_print(lcd_t *lcd, const char *str);
void lcd_set_cursor(lcd_t *lcd, uint8_t row, uint8_t col);


void lcd_display_on(lcd_t *lcd);
void lcd_display_off(lcd_t *lcd);

void lcd_cursor_on(lcd_t *lcd);
void lcd_cursor_off(lcd_t *lcd);

void lcd_blink_on(lcd_t *lcd);
void lcd_blink_off(lcd_t *lcd);

#endif // _LCD_DRIVER_H