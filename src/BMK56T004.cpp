/*****************************************************************
File:        BMK56T004.cpp
Author:      BEST MODULES CORP.
Description: IIC communication with the BMK56T004 and obtain the corresponding value  
Version:     V1.0.3   -- 2025-04-30
******************************************************************/
#include "BMK56T004.h"
/**********************************************************
Description: Constructor
Parameters: intPin :INT Output pin connection with Arduino 
            theWire : Wire object if your board has more than one I2C interface           
Return:     None    
Others:     None     
**********************************************************/
BMK56T004::BMK56T004(uint8_t intPin,TwoWire *theWire)
{
     _intPin = intPin;
     _wire = theWire;
     
}
/**********************************************************
Description: Module Initial
Parameters:     i2c_addr :Module IIC address   
Return:      void    
Others:      None       
**********************************************************/
void BMK56T004::begin(uint8_t i2c_addr)
{
       pinMode(_intPin,INPUT);
       _wire->begin();
       _i2caddr = i2c_addr;
}
/**********************************************************
Description: get FW Ver
Parameters:  void      
Return:      Ver    
Others:      None
**********************************************************/
uint16_t BMK56T004::getFWVer()
{
    uint8_t KeyCMD[1]={0X0B};
    uint8_t fwBuff[2]={0};
    writeBytes(KeyCMD,1); 
    delay(5);
    readBytes(fwBuff,2);
    uint16_t fwValue = (fwBuff[0]<<8)+fwBuff[1];
    delay(10);
    return fwValue;
}
/**********************************************************
Description: get Key Status
Parameters:  void      
Return:      Returns the INT state  
             0:INT output low level  press
             1:INT output high level   unpress    
Others:      None
**********************************************************/
uint8_t BMK56T004::getINT()
{
     return (digitalRead(_intPin));
} 
/**********************************************************
Description: read Key Value
Parameters:  void        
Return:      kvalue:Variables for storing value Key
                    kvalue=0:No key is pressed
                    bit0=1 : key1 is pressed
                    bit1=1 : key2 is pressed
                    bit2=1 : key3 is pressed
                    bit3=1 : key4 is pressed   
Others:      None
**********************************************************/
uint8_t BMK56T004::readKeyValue()
{
  uint8_t value[]={0};
  uint8_t KeyCMD[1]={0X0E};
  writeBytes(KeyCMD,1);
  delay(5);
  readBytes(value,1);
  uint8_t kvalue = value[0];
  delay(10); 
  return kvalue;
}
/**********************************************************
Description: read Wheel Value
Parameters:  void        
Return:      wvalue:Variables for storing value Wheel
                   wvalue=0:The Wheel are untouched
                   wvalue=n:The Wheel n is touched(1~8) 
Others:      None
**********************************************************/
uint8_t BMK56T004::readWheelValue()
{
  uint8_t value[]={0};
  uint8_t WheelCMD[1]={0X0D};
  writeBytes(WheelCMD,1);
  delay(5); 
  readBytes(value,1);
  uint8_t wvalue = value[0];
  delay(10); 
  return wvalue; 
}
/**********************************************************
Description: get Threshold
Parameters:  buff :Store acquired 12 touch threshold 
               buff[0]~buff[7]：Store the threshold of the wvalue at positions 1 to 8
               buff[8]~buff[11]：Store the threshold of key1 to key4      
Return:      void    
Others:      None       
**********************************************************/
void BMK56T004::getThreshold(uint8_t buff[])
{
  writeBytes(TLCMD,1);  
  delay(5); 
  readBytes(buff,12);
  delay(10); 
}
/**********************************************************
Description: set Threshold
Parameters:  buff :Set acquired 12 touch threshold 
               buff[0]~buff[7]：Set the threshold of the wvalue at positions 1 to 8,range: 10-64
               buff[8]~buff[11]：Set the threshold of key1 to key4,range: 10-64          
Return:      Implementation status:
               0:Success 
               1:Fail  
Others:      None 
**********************************************************/
int BMK56T004::setThreshold(uint8_t buff[])
{
  uint16_t total = 0;
  uint8_t TXbuff[14] = {0};
  for(u8 i=0;i<12;i++)                    //sum
  {
      total = total + buff[i];
      TXbuff[i+1] = buff[i];
  }
  if(160 <= total && total<= 1024)//10~64
  {
      TXbuff[0] = 0XD8;
      TXbuff[13] = (uint8_t)total;
      writeBytes(TXbuff,14);
      delay(10);
      return SUCCESS;
  }
  else
  {
    return FAIL;
  }
}
/**********************************************************
Description: set all Threshold Level
Parameters:  level :Storing ThresholdLevel Data,range: 0-6        
Return:      Implementation status:
              0:Success 
              1:Fail  
Others:      threshold = 10+level*9     
**********************************************************/
int BMK56T004::setAllThresholdLevel(uint8_t level)
{
 if(level >= 7)
 {
   return FAIL; 
 }
  uint8_t thr = 10+level*9;
  uint8_t sum = 12*thr;
  uint8_t STLCMD[] = {0xD8,thr,thr,thr,thr,thr,thr,thr,thr,thr,thr,thr,thr,sum};
  writeBytes(STLCMD,14);
  delay(10); 
  return SUCCESS; 
}

/**********************************************************
Description: set Led Mode
Parameters:  mode : 0 - flower key mode
                    1 - CMD Mode
Return:  void 
Others:  none     
**********************************************************/
void BMK56T004::setLedMode(uint8_t mode)
{
  if(mode <= 1)
  {
    uint8_t ledStatusCMD[3] = {0x02,0,0};  //Clear the LED status to avoid the light from turning on when setting the CMD mode
    writeBytes(ledStatusCMD,3); 
    uint8_t ledModeCMD[2] = {0x01,mode};
    writeBytes(ledModeCMD,2);
    delay(10); 
  }
}

/**********************************************************
Description: get Led Mode
Parameters:  void
Return:  Led Mode:
              0:flower key mode 
              1:CMD Mode 
Others:  none     
**********************************************************/
uint8_t BMK56T004::getLedMode()
{
  uint8_t value[1]={0};
  uint8_t ledModeCMD[1]={0X01};
  writeBytes(ledModeCMD,1);
  delay(5);
  readBytes(value,1);
  delay(10); 
  return value[0];
}

/**********************************************************
Description: write Led
Parameters:  data : led status(bit0~bit12：Key1~Key12)
            0: led off
            1: led on
Return:  void 
Others:  none     
**********************************************************/
void BMK56T004::writeLed(uint16_t data)
{
  uint8_t LedCMD[3] = {0x02,(uint8_t)data,(uint8_t)(data>>8)};
  writeBytes(LedCMD,3);
  delay(10); 
}

/**********************************************************
Description: read Led
Parameters:  void
Return:  led status(bit0~bit12：Key1~Key12)
            0: led off
            1: led on
Others:  none     
**********************************************************/
uint16_t BMK56T004::readLed()
{
  uint8_t value[2]={0};
  uint8_t LedCMD[1]={0X02};
  writeBytes(LedCMD,1);
  delay(5);
  readBytes(value,2);
  delay(10); 
  return ((uint16_t)value[1]<<8)+value[0];
}

/**********************************************************
Description: writeBytes
Parameters:  wbuf :the bytes sent
             wlen :the length of the data sent          
Return:      void    
Others:      None        
**********************************************************/
void BMK56T004::writeBytes(uint8_t wbuf[], uint8_t wlen)
{
    while (_wire->available() > 0)
    {
      _wire->read();
    }
    _wire->beginTransmission(_i2caddr);
    _wire->write(wbuf, wlen);
    _wire->endTransmission();
}
/**********************************************************
Description: read Bytes
Parameters:  rbuf :the bytes receive
             rlen :the length of the data receive
Return:      Implementation status:
              0:Success 
              1:Fail  
Others:      None
**********************************************************/
uint8_t BMK56T004::readBytes(uint8_t rbuf[], uint8_t rlen)
{
  _wire->requestFrom(_i2caddr, rlen);
  if(_wire->available()==rlen)
  {
      for (uint8_t i = 0; i < rlen; i++)
      {
        rbuf[i] = _wire->read();
      }
     return SUCCESS;
  }
  else
  {
       return FAIL;
  }
}
