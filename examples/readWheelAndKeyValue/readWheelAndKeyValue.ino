/*****************************************************************
File:         readWheelAndKeyValue.ino
Description:  Connect the IIC interface of BMK56T004 to Wire of Arduino
              and connect the INT interface to PIN2 at the same time. 
              When a button is pressed, Arduino will output the 
              Key+Wheel status through the Serial monitor
******************************************************************/
#include "BMK56T004.h"
BMK56T004     BMK56(2,&Wire);//intPin,Please comment out this line of code if you don't use Wire
//BMK56T004     BMK56(22,&Wire1);//Please uncomment out this line of code if you use Wire1 on BMduino
//BMK56T004     BMK56(25,&Wire2);//Please uncomment out this line of code if you use Wire2 on BMduino

void setup() 
{
  Serial.begin(9600);
  BMK56.begin();
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
