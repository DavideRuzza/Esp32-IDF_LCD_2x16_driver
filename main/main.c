#include <stdio.h>
#include "stdint.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "lcd_driver.h"


#define LOG_LOCAL_LEVEL ESP_LOG_VERBOSE
#include "esp_log.h"

void app_main(void)
{
    // const char* TAG = "LCDTEST";

    lcd_t lcd;
    
    
    //             rs  en  d4  d5  d6  d7
    init_lcd(&lcd, 33, 27, 15, 32, 14, 22);
    lcd_clear(&lcd);
    
    lcd_cursor_off(&lcd);
    lcd_print(&lcd, "Hello World!");
    lcd_set_cursor(&lcd, 1, 0);
    lcd_print(&lcd, "Display test!");
    lcd_cursor_off(&lcd);
    lcd_blink_off(&lcd);

    vTaskDelay(5000/portTICK_PERIOD_MS);
    lcd_clear(&lcd);
    lcd_set_cursor(&lcd, 0, 0);
    lcd_print(&lcd, "Cursor ON");
    lcd_set_cursor(&lcd, 1, 0);
    lcd_print(&lcd, "Blink ON");
    lcd_set_cursor(&lcd, 0, 15);
    lcd_cursor_on(&lcd);
    lcd_blink_on(&lcd);

    vTaskDelay(5000/portTICK_PERIOD_MS);
    lcd_clear(&lcd);
    lcd_set_cursor(&lcd, 0, 0);
    lcd_print(&lcd, "Cursor ON");
    lcd_set_cursor(&lcd, 1, 0);
    lcd_print(&lcd, "Blink OFF");
    lcd_set_cursor(&lcd, 0, 15);
    lcd_cursor_on(&lcd);
    lcd_blink_off(&lcd);

    vTaskDelay(5000/portTICK_PERIOD_MS);
    lcd_clear(&lcd);
    lcd_set_cursor(&lcd, 0, 0);
    lcd_print(&lcd, "Cursor OFF");
    lcd_set_cursor(&lcd, 0, 15);
    lcd_cursor_off(&lcd);
    lcd_blink_off(&lcd);

    vTaskDelay(5000/portTICK_PERIOD_MS);
    lcd_clear(&lcd);
    lcd_set_cursor(&lcd, 0, 0);
    lcd_print(&lcd, "Bye Bye!");
}
