/**************************Eleunit ESP32 Pixel Example Code************************
Version     :	1.0
Suitable for:	Eleunit ESP32 Pixel
Product link:	https://www.eleunit.com/product/esp32-pixel/
Code	  link:	https://github.com/Eleunit/ESP32_Pixel_Tutorial
Lesson	link:	https://www.youtube.com/@Eleunit
Description	:	The code is currently available based on the course on YouTube, 
				        if you have any questions, please refer to the course video: Introduction 
				        to ask questions or feedback.
**************************************************************/




#include <Wire.h>
#include <SPI.h>
#include <PCA9557.h>


/*******************************************************************************
   Config the display panel and touch panel in gfx_conf.h
 ******************************************************************************/
#include "gfx_config.h"

PCA9557 Out;

void setup()
{
  Serial.begin(115200);

  //touch timing init
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

  //Display Prepare
  tft.begin();
  tft.fillScreen(TFT_BLACK);
  tft.setTextSize(3);
  delay(100);

  tft.fillScreen(TFT_RED);
  delay(1000);
  tft.fillScreen(TFT_YELLOW);
  delay(1000);
  tft.fillScreen(TFT_GREEN);
  delay(1000);
  tft.fillScreen(TFT_WHITE);
  delay(1000);
  tft.fillScreen(TFT_BLACK);
  tft.fillCircle( 100, 100, 50, TFT_YELLOW);
  tft.setCursor(200, 240);
  tft.print("Hello, Eleunit");

}
 
void loop()
{
  uint16_t touchX, touchY;
  bool touched = tft.getTouch( &touchX, &touchY);
  if( touched )
  {
    tft.fillCircle(touchX, touchY, 10, TFT_WHITE);
    Serial.print( "Data x " );
    Serial.println( touchX );

    Serial.print( "Data y " );
    Serial.println( touchY );
  }
}


