#include "ldr.h"
#include "Arduino.h"
#include "main.h"

void init_ldr(void)
{
   pinMode(GARDEN_LIGHT, OUTPUT);
   
}

unsigned int Light_intensity,duty;

void brightness_control(void)
{
   // read light intensity from LDR
  Light_intensity = analogRead(LDR_SENSOR);

  //scale down the light intensity or map the values from 1 to 1023 and 255 to 0
  duty = 255 - (Light_intensity / 4);

  //Control the brightness of the LED
  analogWrite(GARDEN_LIGHT, duty);

  delay(100);

}
