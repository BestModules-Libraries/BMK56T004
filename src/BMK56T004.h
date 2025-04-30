/*****************************************************************
File:        BMK56T004.h
Author:      BEST MODULES CORP.
Description: IIC communication with the BMK56T004 and obtain the corresponding value  
Version:     V1.0.3   -- 2025-04-30
******************************************************************/

#ifndef _BMK56T004_H_
#define _BMK56T004_H_

#include <Wire.h>
#include <Arduino.h>

#define SUCCESS            0
#define FAIL               1
#define BMK56T004_IICADDR  0x74

class BMK56T004
{
  public:
  BMK56T004(uint8_t intPin,TwoWire *theWire = &Wire);
  void begin(uint8_t i2c_addr = BMK56T004_IICADDR);
  uint16_t getFWVer();
  uint8_t getINT();
  uint8_t readKeyValue();
  uint8_t readWheelValue();
  void getThreshold(uint8_t buff[]);
  int setThreshold(uint8_t buff[]);
  int setAllThresholdLevel(uint8_t level);

  void setLedMode(uint8_t mode);
  uint8_t getLedMode();
  void writeLed(uint16_t data);
  uint16_t readLed();

  private:
  void writeBytes(uint8_t wbuf[], uint8_t wlen);
  uint8_t readBytes(uint8_t rbuf[], uint8_t rlen);

  
  uint8_t _intPin;
  uint8_t _i2caddr;
  TwoWire *_wire;
  uint8_t TLCMD[1] = {0xD8};//threshold CMD
};



#endif
