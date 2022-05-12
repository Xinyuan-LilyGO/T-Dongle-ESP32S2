#include "pin_config.h"
#include "picoImage.h"
#include <TFT_eSPI.h> // Graphics and font library for ST7735 driver chip
#include <SPI.h>
#include "FS.h"
#include "SD.h"
#include <Wire.h>
#include "WiFi.h"
// #include <U8g2lib.h>

TFT_eSPI tft = TFT_eSPI(); // Invoke library, pins defined in User_Setup.h
SPIClass hspi(HSPI);
// U8G2_SSD1306_128X64_NONAME_F_SW_I2C u8g2(U8G2_R0, IIC_SCL, IIC_SDA, U8X8_PIN_NONE);
int x, y = 0;

const char *ssid = "Your-ssid";
const char *password = "Your-password";

const char *ntpServer1 = "ntp1.aliyun.com";
const char *ntpServer2 = "pool.ntp.org";
const long gmtOffset_sec = 60 * 60 * 8;
const int daylightOffset_sec = 0;

void print_psram_and_flash()
{
  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_GREEN, TFT_BLACK);
  tft.setCursor(x, y);
  tft.print("Chip: ");
  tft.println(ESP.getChipModel());
  tft.print("ChipRevision: ");
  tft.println(ESP.getChipRevision());
  tft.print("Psram size: ");
  tft.println(ESP.getPsramSize());
  tft.print("Flash size: ");
  tft.println(ESP.getFlashChipSize());
}

void scan_iic_test()
{
  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_GREEN, TFT_BLACK);
  tft.setCursor(x, y);

  Wire.begin(IIC_SDA, IIC_SCL);
  uint8_t error, address;
  int nDevices;
  tft.println("I2C Scanner Scanning...");
  nDevices = 0;
  for (address = 1; address < 127; address++)
  {
    Wire.beginTransmission(address);
    error = Wire.endTransmission();
    if (error == 0)
    {
      tft.print("I2C device found at address 0x");
      if (address < 16)
        tft.print("0");
      tft.print(address, HEX);
      tft.println(" !");
      nDevices++;
    }
    else if (error == 4)
    {
      tft.print("Unknow error at address 0x");
      if (address < 16)
        tft.print("0");
      tft.println(address, HEX);
    }
  }
  if (nDevices == 0)
    tft.println("No I2C devices found\n");
  else
    tft.println("done\n");
}

void sd_card_test()
{
  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_GREEN, TFT_BLACK);
  tft.setCursor(x, y);
  tft.println("Initializes the SD Card");

  hspi.begin(SD_PIN_NUM_CLK, SD_PIN_NUM_MISO, SD_PIN_NUM_MOSI, SD_PIN_NUM_CS);
  uint8_t i = 0;
  while (!SD.begin(SD_PIN_NUM_CS, hspi))
  {
    tft.setTextColor(TFT_RED, TFT_BLACK);
    tft.println("Card Mount Failed");
    delay(100);
    if (++i > 3)
    {
      break;
    }
  }
  uint8_t cardType = SD.cardType();

  if (cardType == CARD_NONE)
  {
    tft.setTextColor(TFT_RED, TFT_BLACK);
    tft.println("No SD_MMC card attached");
  }
  tft.print("SD_MMC Card Type: ");
  if (cardType == CARD_MMC)
    tft.println("MMC");
  else if (cardType == CARD_SD)
    tft.println("SDSC");
  else if (cardType == CARD_SDHC)
    tft.println("SDHC");
  else
    tft.println("UNKNOWN");

  uint32_t cardSize = SD.cardSize() / (1024 * 1024);
  String str = "SDCard Size: " + String(cardSize) + "MB";
  tft.println(str);
}

void wifi_test()
{
  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_GREEN, TFT_BLACK);
  tft.setCursor(x, y);
  tft.println("Initializes the WIFI");
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);
  tft.println("scan start");
  int n = WiFi.scanNetworks();
  Serial.println("scan done");
  if (n == 0)
  {
    tft.setTextColor(TFT_RED, TFT_BLACK);
    Serial.println("no networks found");
  }
  else
  {
    tft.fillScreen(TFT_BLACK);
    tft.setCursor(x, y);
    tft.print(n);
    tft.println(" networks found");
    if (n > 7)
      tft.setTextFont(1);
    for (int i = 0; i < n; ++i)
    {
      tft.print(i + 1);
      tft.print(": ");
            char ssid[35];
      WiFi.SSID(i).toCharArray(ssid, 33);
      tft.print(ssid);
//      tft.print(WiFi.SSID(i).c_str());
      tft.print(" (");
      tft.print(WiFi.RSSI(i));
      tft.print(")");
      tft.println((WiFi.encryptionType(i) == WIFI_AUTH_OPEN) ? " " : "*");
      delay(10);
    }
  }
  tft.println();
  // Wait a bit before scanning again
  delay(3000);
  tft.setTextFont(2);
  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_GREEN, TFT_BLACK);
  tft.setCursor(x, y);
  tft.print("Connecting to ");
  tft.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    tft.print(".");
  }
  tft.println("");
  tft.println("WiFi connected");
  tft.print("IP address: ");
  tft.println(WiFi.localIP());
  struct tm timeinfo;
  // Synchronize RTC time
  tft.println("Synchronize RTC time");
  while (!getLocalTime(&timeinfo))
  {
    // init and get the time
    configTime(gmtOffset_sec, daylightOffset_sec, ntpServer1, ntpServer2);
    // tft.println("Failed to obtain time , try again");
    tft.print(".");
  }
  WiFi.disconnect(true);
  WiFi.mode(WIFI_OFF);
}

void setup(void)
{
  Serial.begin(115200);

  u8g2.begin();
  u8g2.clearBuffer();
  u8g2.drawXBM(0, 11, 128, 41, oled_Image);
  u8g2.sendBuffer();

  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);

  tft.init();
  tft.setRotation(1);
  tft.setSwapBytes(true);
  //tft.loadFont(FreeMono9pt7bBitmaps);
  tft.setTextFont(2);
  // tft.invertDisplay(false);
  tft.fillScreen(TFT_BLACK);
  tft.pushImage(0, 0, 240, 135, Lilygo1);

  ledcSetup(0, 4000, 8);
  ledcAttachPin(TFT_BL, 0);
  for (int dutyCycle = 0; dutyCycle <= 255; dutyCycle = dutyCycle + 5) //逐渐点亮
  {
    ledcWrite(0, dutyCycle);
    delay(5);
  }
  delay(3000);
  print_psram_and_flash();
  delay(3000);
  scan_iic_test();
  delay(3000);
  sd_card_test();
  delay(3000);
  wifi_test();
  tft.fillScreen(TFT_BLACK);
  tft.setFreeFont(&FreeMono18pt7b);
}

void loop()
{
  char buf[20];
  struct tm timeinfo;
  if (getLocalTime(&timeinfo))
  {
    tft.fillScreen(TFT_BLACK);
    memset(buf, '\0', sizeof(buf));
    sprintf(buf, "%04d-%02d-%02d", timeinfo.tm_year + 1900, timeinfo.tm_mon + 1, timeinfo.tm_mday);
    tft.drawString(buf, 0, 41, 1);

    memset(buf, '\0', sizeof(buf));
    sprintf(buf, "%02d-%02d-%02d", timeinfo.tm_hour, timeinfo.tm_min, timeinfo.tm_sec);
    tft.drawString(buf, 0, 70, 1);
  }

  digitalWrite(LED_PIN, HIGH);
  delay(20);
  digitalWrite(LED_PIN, LOW);
  delay(980);
}
