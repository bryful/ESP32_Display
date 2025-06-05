// #include <LittleFS.h>
#include <vector>
#include "DHTesp.h" // Click here to get the library: http://librarymanager/All#DHTesp
// #include <Adafruit_Sensor.h>
// #include <DHT.h>
// #include <DHT_U.h>

#include "LGFX_esp32_s3_fh4r2_SPI_ST7789.hpp"
// #include "LGFX_esp32_s3_fh4r2_SPI_ST7789_28.hpp"
#include "FsSerial.hpp"

// 準備したクラスのインスタンスを作成します。
LGFX_esp32_s3_fh4r2_SPI_ST7789 display;
DHTesp dht;
//  Uncomment the type of sensor in use:
// #define DHTTYPE DHT11 // DHT 11
// #define DHTTYPE    DHT22     // DHT 22 (AM2302)
// #define DHTTYPE    DHT21     // DHT 21 (AM2301)
// #define DHTPIN 5 // Digital pin connected to the DHT sensor

// DHT_Unified dht(DHTPIN, DHTTYPE);
unsigned long mm = 0;
void PrintData()
{
  display.startWrite();
  display.fillScreen(TFT_BLACK);
  display.setTextSize(4);
  display.setTextColor(TFT_RED);
  display.setCursor(5, 20);
  display.printf("t:%1.2f", dht.getTemperature());
  display.setCursor(5, 50);
  display.printf("h:%1.2f", dht.getHumidity());
  display.setCursor(5, 80);
  display.printf("%s", dht.getStatusString());
  display.setCursor(5, 110);
  display.printf("%d", dht.getPin());
  display.endWrite();
}
// -----------------------------------------------------------------------------
void GetSerialCMD()
{
  if (Serial.available() < 8)
    return;
  BrySerial srl;

  char header[4];
  for (int i = 0; i < 4; ++i)
  {
    header[i] = (char)Serial.read();
  }
  uint32_t rsize = 0;
  const size_t maxSize = 1024; // 最大100KB
  for (int i = 0; i < 4; ++i)
  {
    rsize |= Serial.read() << (i * 8);
  }

  int err = 0;
  int cnt = 0;
  std::vector<uint8_t> dataBuffer;
  if ((rsize > 0) && (rsize < 1024))
  {
    while (dataBuffer.size() < rsize)
    {
      if (Serial.available())
      {
        dataBuffer.push_back(Serial.read());
        cnt++;
      }
      else
      {
        err++;
        if (err > 1000)
        {
          err = -1;
          break;
        }
        delay(10);
      }
    }
  }
  if (srl.compHeader(header, (char *)"m5sk") == true)
  {
    char str[] = "M5Stack AtomS3";
    srl.SendBin("m5sk", (uint8_t *)str, strlen(str));
  }

  else if (srl.compHeader(header, (char *)"text") == true)
  {

    // ExtPrintln(String((char *)dataBuffer.data()));
    srl.SendText("return:" + String((char *)dataBuffer.data()));
    // aaa = true;
  }

  else
  {
    // ExtPrintln("header:" + String(header) + " size:" + String(rsize) + " cnt:" + String(cnt));
  }
}

void setup(void)
{
  Serial.begin(115200);
  Serial.setTimeout(2000);
  // myTransfer.begin(Serial);

  display.init();

  display.fillScreen(TFT_RED);

  // LittleFS.begin();
  //  display.drawJpgFile(LittleFS, "/mimi.jpg", 0, 0);
  display.setRotation(1);
  display.setTextSize(2);
  display.setTextColor(TFT_WHITE);

  dht.setup(1, DHTesp::DHT11); // Connect DHT sensor to GPIO 17
  PrintData();
  mm = millis() + 2000;
}

void loop(void)
{

  GetSerialCMD();
  unsigned long now = millis();
  if (now > mm)
  {
    PrintData();
    mm = millis() + 2000;
  }
}