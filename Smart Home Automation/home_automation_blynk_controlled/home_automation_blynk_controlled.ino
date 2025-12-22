/*************************************************************
Title         :   Home automation using blynk
Description   :   To control light's brigntness with brightness,monitor temperature , monitor water level in the tank through blynk app
Pheripherals  :   Arduino UNO , Temperature system, LED, LDR module, Serial Tank, Blynk cloud, Blynk App.
 *************************************************************/

// Template ID, Device Name and Auth Token are provided by the Blynk.Cloud
// See the Device Info tab, or Template settings
#define BLYNK_TEMPLATE_ID "TMPL31cK11rws"
#define BLYNK_TEMPLATE_NAME "Home Automation"
#define BLYNK_AUTH_TOKEN "**************"


// // Comment this out to disable prints 
// #define BLYNK_PRINT Serial

#include <SPI.h>
#include <Ethernet.h>
#include <BlynkSimpleEthernet.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#include "main.h"
#include "temperature_system.h"
#include "ldr.h"
#include "serial_tank.h"

bool heater_sw,inlet_sw,outlet_sw,cooler_sw;


//Creating an object  called timer to read the temperature on the blynk app at a particular time
BlynkTimer timer;

LiquidCrystal_I2C lcd(0x27, 16, 2); // set the LCD address to 0x27 for a 16 chars and 2 line display

// This function is called every time the Virtual Pin 0 state changes
/*To turn ON and OFF cooler based virtual PIN value*/
BLYNK_WRITE(COOLER_V_PIN)
{
  //Read the changed value on the pin
  cooler_sw = param.asInt();
  //Based on the pin val control the cooler 
  cooler_control(cooler_sw);
  if(cooler_sw)
  {
    lcd.setCursor(8,0);
    lcd.print("CLR_ON ");
  }
  else
  {
    lcd.setCursor(8,0);
    lcd.print("CLR_OFF");
  }
}
/*To turn ON and OFF heater based virtual PIN value*/
BLYNK_WRITE(HEATER_V_PIN)
{
  //Read the changed value on the pin
  heater_sw = param.asInt();
  //Based on the pin val control the heater
  heater_control(heater_sw);
  if(heater_sw)
  {
    lcd.setCursor(8,0);
    lcd.print("HTR_ON");
  }
  else
  {
    lcd.setCursor(8,0);
    lcd.print("HTR_OFF");
  }

}
/*To turn ON and OFF inlet vale based virtual PIN value*/
BLYNK_WRITE(INLET_V_PIN)
{
  inlet_sw = param.asInt();
  if(inlet_sw)
  {
    enable_inlet();
    lcd.setCursor(7,1); //printing on the lcd
    lcd.print("IN_FL_ON ");

  }
  else
  {
    disable_inlet();
    lcd.setCursor(7,1); //printing on the lcd
    lcd.print("IN_FL_OFF");

  }
  
}
/*To turn ON and OFF outlet value based virtual switch value*/
BLYNK_WRITE(OUTLET_V_PIN)
{
  outlet_sw = param.asInt();
  if(outlet_sw)
  {
    enable_outlet();
    lcd.setCursor(7,1); //printing on the lcd
    lcd.print("OT_FL_ON ");

  }
  else
  {
    disable_outlet();
    lcd.setCursor(7,1); //printing on the lcd
    lcd.print("OT_FL_OFF");

  }
}
/* To display temperature and water volume as gauge on the Blynk App*/  
void update_temperature_reading()
{
  // to send the current Temperature to the Blynk mobile app
  Blynk.virtualWrite(TEMPERATURE_GAUGE, read_temperature());

  //Send volume of the water to the blynk app on pin volume gauge
  Blynk.virtualWrite(WATER_VOL_GAUGE, volume());
  
  
}

/*To turn off the heater if the temperature raises above 35 deg C*/
 void handle_temp(void)
 {
   if(read_temperature() > (float)35 && heater_sw)
   {
     // Turn off the heater switch on the blynk app
     heater_sw = 0;
     //Heater will goes off
     heater_control(0);
     Blynk.virtualWrite(HEATER_V_PIN, 0);
     //Print Notification on the CLCD
     lcd.setCursor(8, 0);
     lcd.print("HTR_OFF");
   }
  
 }

/*To control water volume less 1000ltrs*/
void handle_tank(void)
{
  // if volume of water is less than 1000, inlet valve is on
 
  if((volume() < 1000) && (inlet_sw == 0))
  {
    // Turn on the inlet valveand update the same on the blynk app
    inlet_sw = 1;
    enable_inlet();
    Blynk.virtualWrite(INLET_V_PIN, 1);
    //Print Notification on the CLCD
    lcd.setCursor(7, 1);
    lcd.print("IN_FL_ON ");
  }
   if((volume() >= 3000) && (inlet_sw == 1))
  {
    // Turn on the inlet valveand update the same on the blynk app
    inlet_sw = 0;
    disable_inlet();
    Blynk.virtualWrite(INLET_V_PIN, 0);
    //Print Notification on the CLCD
    lcd.setCursor(7, 1);
    lcd.print("IN_FL_OFF ");
  }
}


void setup(void)
{
  

  //initializing the LDR Sensor
  init_ldr();

  //initializing the temperature system
  init_temperature_system();
   // Connecting to the Blynk app
  Blynk.begin(BLYNK_AUTH_TOKEN);

  //initializing the CLCD 
  lcd.init();
  lcd.backlight();
  lcd.clear();
  lcd.home();
  lcd.setCursor(0, 0);
  lcd.print("Home Automation");
  delay(1000);
  lcd.clear();
  //Initializing the serial tank
  init_serial_tank();

  //updating the temperature for every 1 second because it cannot read the 10 values at one second
  timer.setInterval(1000,update_temperature_reading);
}

float temp;
unsigned int volume_water;
void loop(void) 
{
 
  brightness_control();

  Blynk.run();
  timer.run();

   //Read the temperature from the temperature sensor i.e., LM35
   temp = read_temperature();
  
  // if temperature increases more than 35 
  handle_temp();
  //if volume of water goes less than 2000
  handle_tank();

  //Reading the volume of the water
  volume_water = volume();

  //displaying the volume of the water  on the CLCD
  lcd.setCursor(0, 1);
  lcd.print(volume_water);
 
  //displaying the temperature on the CLCD
  lcd.setCursor(0, 0);
  lcd.print(temp);
  
}   
