/*****************************************************************
File:         writeLED.ino
Description:  (1)Set the LED mode to CMD mode
              (2)Display LED flow light
              (3)Set the LED mode to key mode
******************************************************************/
#include "BMK56T004.h"
BMK56T004     BMK56(2,&Wire);//intPin,Please comment out this line of code if you don't use Wire
//BMK56T004     BMK56(22,&Wire1);//Please uncomment out this line of code if you use Wire1 on BMduino
//BMK56T004     BMK56(25,&Wire2);//Please uncomment out this line of code if you use Wire2 on BMduino
uint16_t led_data = 0;
void setup() 
{
  Serial.begin(9600);
  BMK56.begin();

  BMK56.setLedMode(1);   //Set the LED mode to CMD mode
  if(BMK56.getLedMode() == 1)
  {
    Serial.println("Set the LED mode to CMD mode : SUCCESS");
  }

  led_data = 0x0001;
  for(uint8_t i=0;i<12;i++) //LED flow light
  {
    BMK56.writeLed(led_data);
    led_data = led_data<<1;
    delay(100);
  }

  BMK56.setLedMode(0);   //Set the LED mode to Key mode
  if(BMK56.getLedMode() == 0)
  {
    Serial.println("Set the LED mode to Key mode : SUCCESS");
  }
}

void loop() 
{
  if(BMK56.getINT() == 0)
  {
      Serial.print("wheelValue:");
      Serial.println(BMK56.readWheelValue());            

      Serial.print("keyValue:");
      Serial.println(BMK56.readKeyValue());       
  }
}
