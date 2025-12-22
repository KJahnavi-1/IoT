#include "temperature_system.h"
#include "Arduino.h"
#include "main.h"


void init_temperature_system(void)
{
 // Setting heater and cooler GPIO Pins as output   
 pinMode(HEATER, OUTPUT);
 pinMode(COOLER, OUTPUT);
    
}

float temperature;
float read_temperature(void)
{
  //Read the temperature sensor and return the temperature
  //read value from A0 pin (0 to 1023)-> vol = adc_val * step size and temp = vol / 10mv
  temperature = (((analogRead(A0)*(float)5/1024))/(float) 0.01);
  return temperature; 
}

//Receiving the state of the button on the mobile Blynk app
void cooler_control(bool control)
{
   if(control)
   {
    //Turn on the cooler -> assigning the digital value 1 to the GPIO pin
    digitalWrite(COOLER, 1);
   }
   else
   {
    //Turn off the Cooler -> assigning the digital value 0 to the GPIO pin
    digitalWrite(COOLER, 0);
   }

}
void heater_control(bool control)
{
  if(control)
  {
    //Turn on the heater -> assigning the digital value 1 to the GPIO pin
    digitalWrite(HEATER, 1);
  }
  else
  {
   //Turn off the heater -> assigning the digital value 0 to the GPIO pin
   digitalWrite(HEATER, 0); 
  }
    
}
