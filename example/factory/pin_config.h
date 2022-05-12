#pragma once

/********************************* USE TFT_eSPI Lib ************************************/
// #define ST7789_DRIVER     // Configure all registers
// #define TFT_WIDTH  135
// #define TFT_HEIGHT 240
// #define CGRAM_OFFSET      // Library will add offsets required
// #define LOAD_GLCD   // Font 1. Original Adafruit 8 pixel font needs ~1820 bytes in FLASH
// #define LOAD_FONT2  // Font 2. Small 16 pixel high font, needs ~3534 bytes in FLASH, 96 characters
// #define LOAD_FONT4  // Font 4. Medium 26 pixel high font, needs ~5848 bytes in FLASH, 96 characters
// #define LOAD_FONT6  // Font 6. Large 48 pixel font, needs ~2666 bytes in FLASH, only characters 1234567890:-.apm
// #define LOAD_FONT7  // Font 7. 7 segment 48 pixel font, needs ~2438 bytes in FLASH, only characters 1234567890:.
// #define LOAD_FONT8  // Font 8. Large 75 pixel font needs ~3256 bytes in FLASH, only characters 1234567890:-.
// #define LOAD_GFXFF  // FreeFonts. Include access to the 48 Adafruit_GFX free fonts FF1 to FF48 and custom fonts

// #define SMOOTH_FONT
// #define SPI_FREQUENCY  40000000
// #define SPI_READ_FREQUENCY  20000000
// #define SPI_TOUCH_FREQUENCY  2500000

// #define TFT_MISO -1
// #define TFT_MOSI 35
// #define TFT_SCLK 36
// #define TFT_CS 34  // Chip select control pin
// #define TFT_DC 37  // Data Command control pin
// #define TFT_RST 38 // Reset pin (could connect to RST pin)
#define TFT_BL 33

#define SD_PIN_NUM_MISO 13
#define SD_PIN_NUM_MOSI 11
#define SD_PIN_NUM_CLK 12
#define SD_PIN_NUM_CS 10

#define LED_PIN 39

#define IIC_SDA 44
#define IIC_SCL 43