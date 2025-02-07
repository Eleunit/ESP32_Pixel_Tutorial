/******************************************************************************/
#include <FS.h>
#include <SD.h>
#include <SPI.h>
#include <Wire.h>
#include <PCA9557.h>
#include "gfx_config.h"

//Modify the corresponding pin according to the circuit diagram.
#define SD_MOSI 11
#define SD_MISO 13
#define SD_SCK 12

PCA9557 Out;

SPIClass SD_SPI;

/******************************************************************************/


// SD test
void listDir(fs::FS & fs, const char *dirname, uint8_t levels)
{
  Serial.printf("Listing directory: %s\n", dirname);

  File root = fs.open(dirname);
  if (!root)
  {
    Serial.println("Failed to open directory");
    return;
  }
  if (!root.isDirectory())
  {
    Serial.println("Not a directory");
    return;
  }

  File file = root.openNextFile();
  while (file)
  {
    if (file.isDirectory())
    {
      Serial.print("  DIR : ");
      Serial.println(file.name());
      if (levels)
      {
        listDir(fs, file.name(), levels - 1);
      }
    }
    else
    {
      Serial.print("  FILE: ");
      Serial.print(file.name());
      Serial.print("  SIZE: ");
      Serial.println(file.size());
    }
    file = root.openNextFile();
  }
}


int sd_init()
{
  SD_SPI.begin(SD_SCK, SD_MISO, SD_MOSI, -1);
  if (!SD.begin(-1, SD_SPI, 120000000))
  {
    Serial.println("Card Mount Failed");
    return 1;
  }
  else
  {
    Serial.println("Card Mount Successed");
  }
  listDir(SD, "/", 0);
  Serial.println("TF Card init over.");
  return 0;
}

void setup()
{
  Serial.begin( 9600 ); /*Serial init*/

  Wire.begin(41, 42);
  Out.reset();
  Out.setMode(IO_OUTPUT);
  Out.setState(IO0, IO_LOW);
  Out.setState(IO1, IO_LOW);
  delay(20);
  Out.setState(IO0, IO_HIGH);
  delay(100);
  Out.setMode(IO1, IO_INPUT);
  Out.setState(IO3, IO_HIGH);
  Out.setState(IO2, IO_LOW);

  // Init Display
  tft.begin();
  tft.setTextSize(2);
  tft.fillScreen(TFT_BLACK);

  if (sd_init() == 0)
  {
    Serial.println("TF init success");
  } else {
    Serial.println("TF init fail");
  }

}


void loop() {
  //display images you place in SD Card
  displayPhoto("/sub/hello.bmp",0,0);
  delay(1000);
  displayPhoto("/2.bmp",0,0);
  delay(1000);
  displayPhoto("/3.bmp",0,0);
  delay(1000);
  displayPhoto("/4.bmp",0,0);
  delay(1000);
  displayPhoto("/5.bmp",0,0);
  delay(1000);
  // displayPhoto("/6.bmp",0,0);
  // delay(1000);
  //displayPhoto("/image.bmp",0,0);
}

void displayPhoto(char* filename, int16_t x, int16_t y) {
  File file = SD.open(filename);

  if (!file) {
    Serial.println("Error opening file!");
    return;
  }

  if (read16(file) != 0x4D42) {
    Serial.println("File is not a BMP file!");
    return;
  }

  read32(file);
  read32(file);
  uint32_t offset = read32(file);

  read32(file);
  uint32_t width = read32(file);
  uint32_t height = read32(file);

  if (read16(file) != 1) {
    Serial.println("File is not a 24-bit bitmap!");
    return;
  }

  file.seek(offset);

  uint16_t rowSize = (width * 3 + 3) & ~3;
  uint8_t row[rowSize];

  for (uint16_t rowNumber = height; rowNumber > 0; rowNumber--) {
    file.read(row, rowSize);

    for (uint16_t colNumber = 0; colNumber < width; colNumber++) {
      uint8_t b = row[3 * colNumber + 0];
      uint8_t g = row[3 * colNumber + 1];
      uint8_t r = row[3 * colNumber + 2];

      tft.drawPixel(colNumber + x, rowNumber - 1 + y, tft.color565(r, g, b));
    }
  }
}

uint16_t read16(File& f) {
  uint16_t result;
  ((uint8_t*)&result)[0] = f.read();
  ((uint8_t*)&result)[1] = f.read();
  return result;
}

uint32_t read32(File& f) {
  uint32_t result;
  ((uint8_t*)&result)[0] = f.read();
  ((uint8_t*)&result)[1] = f.read();
  ((uint8_t*)&result)[2] = f.read();
  ((uint8_t*)&result)[3] = f.read();
  return result;
}


