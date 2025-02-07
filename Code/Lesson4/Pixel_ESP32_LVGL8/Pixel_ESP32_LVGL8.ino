#include <Wire.h>
#include <lvgl.h>
#include <demos/lv_demos.h>
#include <examples/lv_examples.h>
#include <PCA9557.h>
#include "gfx_config.h"

PCA9557 Out;

/*LVGL draw into this buffer, 1/10 screen size usually works well. The size is in bytes*/
static lv_disp_draw_buf_t draw_buf;

static lv_color_t disp_draw_buf1[screenWidth * screenHeight / 8];
static lv_color_t disp_draw_buf2[screenWidth * screenHeight / 8];


/* Display flushing */
void my_disp_flush(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p)
{
   uint32_t w = ( area->x2 - area->x1 + 1 );
   uint32_t h = ( area->y2 - area->y1 + 1 );

   //tft.pushImageDMA(area->x1, area->y1, w, h,(lgfx::rgb565_t*)&color_p->full);

   tft.startWrite();
   tft.setAddrWindow( area->x1, area->y1, w, h );
   //tft.pushColors( ( uint16_t * )&color_p->full, w * h, true );
   tft.writePixels((lgfx::rgb565_t *)&color_p->full, w * h);
   tft.endWrite();

   lv_disp_flush_ready( disp );

}


void my_touchpad_read(lv_indev_drv_t *indev_driver, lv_indev_data_t *data)
{
   uint16_t touchX, touchY;
   bool touched = tft.getTouch( &touchX, &touchY);
   if( touched )
   {
      data->state = LV_INDEV_STATE_PR;

      /*Set the coordinates*/
      data->point.x = touchX;
      data->point.y = touchY;

      Serial.print( "Data x " );
      Serial.println( touchX );

      Serial.print( "Data y " );
      Serial.println( touchY );
   }
   else
   {
      data->state = LV_INDEV_STATE_REL;
   }
}

void lvgl_driver_function_init()
{
  lv_disp_draw_buf_init(&draw_buf, disp_draw_buf1, disp_draw_buf2, screenWidth * screenHeight / 8);

  /* Initialize the display */
  static lv_disp_drv_t disp_drv;
  lv_disp_drv_init(&disp_drv);
  /* Change the following line to your display resolution */
  disp_drv.hor_res = screenWidth;
  disp_drv.ver_res = screenHeight;
  disp_drv.flush_cb = my_disp_flush;
  //disp_drv.full_refresh = 1;
  //disp_drv.direct_mode = 1;
  disp_drv.draw_buf = &draw_buf;
  lv_disp_drv_register(&disp_drv);

  /* Initialize the (dummy) input device driver */
  static lv_indev_drv_t indev_drv;
  lv_indev_drv_init(&indev_drv);
  indev_drv.type = LV_INDEV_TYPE_POINTER;
  indev_drv.read_cb = my_touchpad_read;
  lv_indev_drv_register(&indev_drv);
}

void setup()
{
  Serial.begin( 115200 );

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

  Serial.println("LVGL_Arduino");
  //Display Prepare
  tft.begin();
  tft.fillScreen(TFT_BLACK);
  Serial.println("TFT INIT");
  lv_init();
  Serial.println("LV_init");

  lvgl_driver_function_init();

  //Try out a demo. Don't forget to enable the demos in lv_conf.h. E.g. LV_USE_DEMOS_WIDGETS*/
    lv_demo_widgets();               
  // lv_demo_benchmark();          
  // lv_demo_keypad_encoder();     
  // lv_demo_music();              
  // lv_demo_printer();
  // lv_demo_stress();
}

void loop()
{
  long last_time = millis();

  delay(3);

  lv_timer_handler(); /* let the GUI do its work */
	lv_tick_inc(int(millis() - last_time));
}


















