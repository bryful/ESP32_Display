#include <LittleFS.h>
#include <vector>
// #include "LGFX_esp32_s3_fh4r2_SPI_ST7789.hpp"
#include "LGFX_esp32_s3_fh4r2_SPI_ST7789_28.hpp"
#include "FsSerial.hpp"

// 準備したクラスのインスタンスを作成します。
LGFX_esp32_s3_fh4r2_SPI_ST7789_28 display;

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
  // myTransfer.begin(Serial);

  display.init();

  display.fillScreen(TFT_RED);

  LittleFS.begin();
  display.drawJpgFile(LittleFS, "/mimi.jpg", 0, 0);
  display.setTextSize(2);
  display.setTextColor(TFT_WHITE);
}

void loop(void)
{
  Serial.begin(115200);
  Serial.setTimeout(2000);

  GetSerialCMD();
}