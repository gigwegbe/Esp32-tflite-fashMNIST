
/*
 * menu_images.h
 *
 *  Created on: 30-Nov-2020
 *      Author: George
 *
 *  Brief: Main Functions
 */

#include <Wire.h>
#include "SPI.h"
#include <LovyanGFX.hpp>
#include "makerfabs_pin.h"

//Choice your touch IC
//#define NS2009_TOUCH
#define FT6236_TOUCH

#ifdef NS2009_TOUCH
#include "NS2009.h"
const int i2c_touch_addr = NS2009_ADDR;
#endif

#ifdef FT6236_TOUCH
#include "FT6236.h"
const int i2c_touch_addr = TOUCH_I2C_ADD;
#endif

//SPI control
#define SPI_ON_TFT digitalWrite(LCD_CS, LOW)
#define SPI_OFF_TFT digitalWrite(LCD_CS, HIGH)

struct LGFX_Config
{
    static constexpr spi_host_device_t spi_host = VSPI_HOST;
    static constexpr int dma_channel = 1;
    static constexpr int spi_sclk = LCD_SCK;
    static constexpr int spi_mosi = LCD_MOSI;
    static constexpr int spi_miso = LCD_MISO;
};

static lgfx::LGFX_SPI<LGFX_Config> tft;
static LGFX_Sprite sprite(&tft);
static lgfx::Panel_ILI9488 panel;

int last_pos[2] = {0, 0};
int draw_color = TFT_WHITE;

void setup()
{

    Serial.begin(115200);
    while (!Serial)
        ; // Leonardo: wait for serial monitor
    Serial.println("\n NS2009 test");

    Wire.begin(I2C_SDA, I2C_SCL);
    byte error, address;

    Wire.beginTransmission(i2c_touch_addr);
    error = Wire.endTransmission();

    if (error == 0)
    {
        Serial.print("I2C device found at address 0x");
        Serial.print(i2c_touch_addr, HEX);
        Serial.println("  !");
    }
    else if (error == 4)
    {
        Serial.print("Unknown error at address 0x");
        Serial.println(i2c_touch_addr, HEX);
    }

    pinMode(LCD_CS, OUTPUT);
    SPI_OFF_TFT;
    SPI.begin(SPI_SCK, SPI_MISO, SPI_MOSI);

    //TFT(SPI) init
    SPI_ON_TFT;

    set_tft();
    tft.begin();
    //tft.init();
    
    tft.fillScreen(TFT_BLACK);
    tft.setTextColor(TFT_WHITE);
    tft.setTextSize(1);
    tft.setCursor((LCD_HEIGHT - 150)/ 2, 5); //(115,5)
    tft.println("Welcome to TinyML Project");
    tft.setCursor(20, 50);
    tft.println("Image box");
    tft.setCursor(300, 50);
    tft.drawRect(20, 60, 240, 175, TFT_BLUE); 
    tft.println("Result box");
    tft.setCursor(300, 150);
    tft.drawRect(300, 60, 100, 50, TFT_BLUE); 
    tft.println("Predict box");
    tft.setCursor(115,270);
    tft.drawRect(300, 160, 100, 50, TFT_BLUE);
    tft.println("<<"); 
    tft.setCursor(180,270);
    tft.println(">>");
    
    SPI_OFF_TFT;
}

void loop(){
int pos[2] = {0, 0};
#ifdef NS2009_TOUCH
Serial.println(ns2009_pos(pos));
#endif
#ifdef FT6236_TOUCH
    ft6236_pos(pos);
#endif
 Serial.printf("%d,%d\n", pos[0], pos[1]);

}


void set_tft()

    panel.freq_write = 60000000;
   
    panel.freq_fill = 60000000;
   
    panel.freq_read = 16000000;

    panel.spi_mode = 0;

    panel.spi_mode_read = 0;

    panel.len_dummy_read_pixel = 8;

    panel.spi_read = true;

    panel.spi_3wire = false;

    // LCD(CS)
    panel.spi_cs = LCD_CS;

    // LCD(DC)
    panel.spi_dc = LCD_DC;

    // LCD RST(Reset)
    panel.gpio_rst = LCD_RST;

    // LCD Backlight
    panel.gpio_bl = LCD_BL;

    // PWM
    panel.pwm_ch_bl = -1;

    //true=HIGH / false=LOW
    panel.backlight_level = true;

    // invertDisplay
    panel.invert = true;

    // RGB=true / BGR=false
    panel.rgb_order = false;

    panel.memory_width = LCD_WIDTH;
    panel.memory_height = LCD_HEIGHT;

    panel.panel_width = LCD_WIDTH;
    panel.panel_height = LCD_HEIGHT;

    panel.offset_x = 0;
    panel.offset_y = 0;

    // setRotation
    panel.rotation = 0;

    panel.offset_rotation = 1;

    tft.setPanel(&panel);
}
