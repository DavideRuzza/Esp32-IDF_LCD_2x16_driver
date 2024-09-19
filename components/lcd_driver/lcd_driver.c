#include <stdio.h>
#include "lcd_driver.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "esp_timer.h"


////////////////// funzione delay
#define NOP() asm volatile ("nop")

unsigned long IRAM_ATTR micros() {
    return (unsigned long) (esp_timer_get_time());
}

void IRAM_ATTR delayMicroseconds(uint32_t us){

    uint32_t m = micros();
    if(us){
        uint32_t e = (m+us);
        if (m > e){
            while(micros()>e){
                NOP();
            }
        }
        while(micros()<e){
            NOP();
        }
    }
}
///////////////////////////

void lcd_init(lcd_t *lcd, int rs, int en, int d4, int d5, int d6, int d7){
    lcd->rs = rs;
    lcd->en = en;
    lcd->d4 = d4;
    lcd->d5 = d5;  
    lcd->d6 = d6;
    lcd->d7 = d7;

    gpio_reset_pin(lcd->rs); gpio_set_direction(lcd->rs, GPIO_MODE_OUTPUT);
    gpio_reset_pin(lcd->en); gpio_set_direction(lcd->en, GPIO_MODE_OUTPUT);
    gpio_reset_pin(lcd->d4); gpio_set_direction(lcd->d4, GPIO_MODE_OUTPUT);
    gpio_reset_pin(lcd->d5); gpio_set_direction(lcd->d5, GPIO_MODE_OUTPUT);
    gpio_reset_pin(lcd->d6); gpio_set_direction(lcd->d6, GPIO_MODE_OUTPUT);
    gpio_reset_pin(lcd->d7); gpio_set_direction(lcd->d7, GPIO_MODE_OUTPUT);

    gpio_set_level(lcd->rs, 0);
    gpio_set_level(lcd->en, 0);

    lcd_setting_t *sett = &(lcd->setting);

    sett->DL = LCD_4BITMODE;
    sett->N = LCD_2LINE;   // default 2 lines
    sett->F = LCD_5x8DOTS;

    sett->D = LCD_DISPLAYON;
    sett->C = LCD_CURSORON;
    sett->B = LCD_BLINKON;

    // printf("number of lines %x\n", sett->N);
    uint8_t function_set = CMD_FUNCTIONSET | sett->DL | sett->N | sett->F; // 4bit, 2line, 5x8 dot display
    uint8_t disp_opt = CMD_DISPLAYCONTROL | sett->D | sett->C | sett->B;
    uint8_t entry_mode = CMD_ENTRYMODESET | sett->ID | sett->S;
    // uint8_t disp_curs_disp = CMD_CURS_DISP_SHIFT | sett->SC | sett->RL;
    // initialization sequence from datasheet pag.46

    vTaskDelay(50/portTICK_PERIOD_MS); // at least 40ms

    send_H(lcd, 0x30);
    delayMicroseconds(4500);

    send_H(lcd, 0x30);
    delayMicroseconds(4500);

    send_H(lcd, 0x30);
    delayMicroseconds(150);

    send_H(lcd, 0x20);
    delayMicroseconds(150);
    send_cmd(lcd, function_set);
    send_cmd(lcd, disp_opt);
    send_cmd(lcd, entry_mode);

}

void pulse_en(lcd_t *lcd){
    gpio_set_level(lcd->en, 0);
    delayMicroseconds(1);
    gpio_set_level(lcd->en, 1);
    delayMicroseconds(1); 
    gpio_set_level(lcd->en, 0);
    delayMicroseconds(50);
}


void send_H(lcd_t *lcd, uint8_t word){
    set_cmd_H(lcd, word);
    pulse_en(lcd);
}

void send_L(lcd_t *lcd, uint8_t word){
    set_cmd_L(lcd, word);
    pulse_en(lcd);
}

void send_cmd(lcd_t *lcd, uint8_t instr){

    gpio_set_level(lcd->rs, 0);

    send_H(lcd, instr);
    send_L(lcd, instr);

}

void send_data(lcd_t *lcd, uint8_t data){

    gpio_set_level(lcd->rs, 1);

    send_H(lcd, data);
    send_L(lcd, data);
}

void set_cmd_H(lcd_t *lcd, uint8_t word){
    gpio_set_level(lcd->d7, word>>7 & 0x01);
    gpio_set_level(lcd->d6, word>>6 & 0x01);
    gpio_set_level(lcd->d5, word>>5 & 0x01);
    gpio_set_level(lcd->d4, word>>4 & 0x01);
}

void set_cmd_L(lcd_t *lcd, uint8_t word){
    gpio_set_level(lcd->d7, word>>3 & 0x01);
    gpio_set_level(lcd->d6, word>>2 & 0x01);
    gpio_set_level(lcd->d5, word>>1 & 0x01);
    gpio_set_level(lcd->d4, word    & 0x01);
}


/////////////////////////////////////

void lcd_clear(lcd_t *lcd){
    send_cmd(lcd, CMD_CLEARDISPLAY);
    delayMicroseconds(2000);
}

void lcd_home(lcd_t *lcd){
    send_cmd(lcd, CMD_RETURNHOME);
    delayMicroseconds(2000);
}

void lcd_write(lcd_t *lcd, char c){
    send_data(lcd, (uint8_t)c);
}


void lcd_print(lcd_t *lcd, const char *str) {
    while (*str != '\0') {
        lcd_write(lcd, (uint8_t)*str);
        str++;
    }
}


void lcd_set_cursor(lcd_t *lcd, uint8_t row, uint8_t col) {
    uint8_t address;
    if (row == 0) {
        address = col; // Prima riga, indirizzo 0x00-0x0F
        send_cmd(lcd, 0x80 | address); // Comando per impostare l'indirizzo del cursore
    } else if (row == 1) {
        address = 0x40 + col; // Seconda riga, indirizzo 0x40-0x4F
        send_cmd(lcd, 0x80 | address);
    }
}


void lcd_display_on(lcd_t *lcd){
    lcd_setting_t *sett = &(lcd->setting);
    sett->D = LCD_DISPLAYON;
    uint8_t cmd = CMD_DISPLAYCONTROL | sett->D | sett->C | sett->B;
    send_cmd(lcd, cmd);
}

void lcd_display_off(lcd_t *lcd){
    lcd_setting_t *sett = &(lcd->setting);
    sett->D = LCD_DISPLAYOFF;
    uint8_t cmd = CMD_DISPLAYCONTROL | sett->D | sett->C | sett->B;
    send_cmd(lcd, cmd);
}

void lcd_cursor_on(lcd_t *lcd){
    lcd_setting_t *sett = &(lcd->setting);
    sett->C = LCD_CURSORON;
    uint8_t cmd = CMD_DISPLAYCONTROL | sett->D | sett->C | sett->B;
    send_cmd(lcd, cmd);
}

void lcd_cursor_off(lcd_t *lcd){
    lcd_setting_t *sett = &(lcd->setting);
    sett->C = LCD_CURSOROFF;
    uint8_t cmd = CMD_DISPLAYCONTROL | sett->D | sett->C | sett->B;
    send_cmd(lcd, cmd);
}


void lcd_blink_on(lcd_t *lcd){
    lcd_setting_t *sett = &(lcd->setting);
    sett->B = LCD_BLINKON;
    uint8_t cmd = CMD_DISPLAYCONTROL | sett->D | sett->C | sett->B;
    send_cmd(lcd, cmd);
}

void lcd_blink_off(lcd_t *lcd){
    lcd_setting_t *sett = &(lcd->setting);
    sett->B = LCD_BLINKOFF;
    uint8_t cmd = CMD_DISPLAYCONTROL | sett->D | sett->C | sett->B;
    send_cmd(lcd, cmd);
}
