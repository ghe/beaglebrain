#include "TouchSensor.h"


#define TOUCH_ADDR 0x5A

#define RESET_REG 0x80

#define MHD_R 0x2B
#define NHD_R 0x2C
#define NCL_R   0x2D
#define FDL_R 0x2E
#define MHD_F 0x2F
#define NHD_F 0x30
#define NCL_F 0x31
#define FDL_F 0x32
#define ELE0_T  0x41
#define ELE0_R  0x42
#define ELE1_T  0x43
#define ELE1_R  0x44
#define ELE2_T  0x45
#define ELE2_R  0x46
#define ELE3_T  0x47
#define ELE3_R  0x48
#define ELE4_T  0x49
#define ELE4_R  0x4A
#define ELE5_T  0x4B
#define ELE5_R  0x4C
#define ELE6_T  0x4D
#define ELE6_R  0x4E
#define ELE7_T  0x4F
#define ELE7_R  0x50
#define ELE8_T  0x51
#define ELE8_R  0x52
#define ELE9_T  0x53
#define ELE9_R  0x54
#define ELE10_T 0x55
#define ELE10_R 0x56
#define ELE11_T 0x57
#define ELE11_R 0x58
#define FIL_CFG 0x5D
#define ELE_CFG 0x5E
#define GPIO_CTRL0  0x73
#define GPIO_CTRL1  0x74
#define GPIO_DATA 0x75
#define GPIO_DIR  0x76
#define GPIO_EN   0x77
#define GPIO_SET  0x78
#define GPIO_CLEAR  0x79
#define GPIO_TOGGLE 0x7A
#define ATO_CFG0  0x7B
#define ATO_CFGU  0x7D
#define ATO_CFGL  0x7E
#define ATO_CFGT  0x7F


#define TOU_THRESH 0x09
#define REL_THRESH 0x05

TouchSensor::TouchSensor()
{
  //Set stop mode
  _i2c.Send_I2C_Byte(TOUCH_ADDR, ELE_CFG, 0x00); //0x5e

  // Section A - Controls filtering when data is > baseline.
  _i2c.Send_I2C_Byte(TOUCH_ADDR, MHD_R, 0x01); //2B
  _i2c.Send_I2C_Byte(TOUCH_ADDR, NHD_R, 0x01); //2C
  _i2c.Send_I2C_Byte(TOUCH_ADDR, NCL_R, 0x00); //2D
  _i2c.Send_I2C_Byte(TOUCH_ADDR, FDL_R, 0x00); //2E

  // Section B - Controls filtering when data is < baseline.
  _i2c.Send_I2C_Byte(TOUCH_ADDR, MHD_F, 0x01); //2F
  _i2c.Send_I2C_Byte(TOUCH_ADDR, NHD_F, 0x01); //30
  _i2c.Send_I2C_Byte(TOUCH_ADDR, NCL_F, 0xFF); //31
  _i2c.Send_I2C_Byte(TOUCH_ADDR, FDL_F, 0x02); //32

  // Section C - Sets touch and release thresholds for each electrode
  _i2c.Send_I2C_Byte(TOUCH_ADDR, ELE0_T, TOU_THRESH); //41
  _i2c.Send_I2C_Byte(TOUCH_ADDR, ELE0_R, REL_THRESH); //21

  _i2c.Send_I2C_Byte(TOUCH_ADDR, ELE1_T, TOU_THRESH);
  _i2c.Send_I2C_Byte(TOUCH_ADDR, ELE1_R, REL_THRESH);

  _i2c.Send_I2C_Byte(TOUCH_ADDR, ELE2_T, TOU_THRESH);
  _i2c.Send_I2C_Byte(TOUCH_ADDR, ELE2_R, REL_THRESH);

  _i2c.Send_I2C_Byte(TOUCH_ADDR, ELE3_T, TOU_THRESH);
  _i2c.Send_I2C_Byte(TOUCH_ADDR, ELE3_R, REL_THRESH);

  _i2c.Send_I2C_Byte(TOUCH_ADDR, ELE4_T, TOU_THRESH);
  _i2c.Send_I2C_Byte(TOUCH_ADDR, ELE4_R, REL_THRESH);

  _i2c.Send_I2C_Byte(TOUCH_ADDR, ELE5_T, TOU_THRESH);
  _i2c.Send_I2C_Byte(TOUCH_ADDR, ELE5_R, REL_THRESH);

  _i2c.Send_I2C_Byte(TOUCH_ADDR, ELE6_T, TOU_THRESH);
  _i2c.Send_I2C_Byte(TOUCH_ADDR, ELE6_R, REL_THRESH);

  _i2c.Send_I2C_Byte(TOUCH_ADDR, ELE7_T, TOU_THRESH);
  _i2c.Send_I2C_Byte(TOUCH_ADDR, ELE7_R, REL_THRESH);

  _i2c.Send_I2C_Byte(TOUCH_ADDR, ELE8_T, TOU_THRESH);
  _i2c.Send_I2C_Byte(TOUCH_ADDR, ELE8_R, REL_THRESH);

  _i2c.Send_I2C_Byte(TOUCH_ADDR, ELE9_T, TOU_THRESH);
  _i2c.Send_I2C_Byte(TOUCH_ADDR, ELE9_R, REL_THRESH);

  _i2c.Send_I2C_Byte(TOUCH_ADDR, ELE10_T, TOU_THRESH);
  _i2c.Send_I2C_Byte(TOUCH_ADDR, ELE10_R, REL_THRESH);

  _i2c.Send_I2C_Byte(TOUCH_ADDR, ELE11_T, TOU_THRESH);
  _i2c.Send_I2C_Byte(TOUCH_ADDR, ELE11_R, REL_THRESH);

  // Section D
  // Set the Filter Configuration
  // Set ESI2
  _i2c.Send_I2C_Byte(TOUCH_ADDR, FIL_CFG, 0x04); //5D

  // Section E
  // Electrode Configuration
  _i2c.Send_I2C_Byte(TOUCH_ADDR, ELE_CFG, 0x0C); //5E  // Enables all 12 Electrodes

  // Section F
  // Enable Auto Config and auto Reconfig
  _i2c.Send_I2C_Byte(0x5A, ATO_CFG0, 0x0B);
  _i2c.Send_I2C_Byte(0x5A, ATO_CFGU, 0xC9);  // USL = (Vdd-0.7)/vdd*256 = 0xC9 @3.3V
  //_i2c.Send_I2C_Byte(0x5A, ATO_CFGL, 0x82);  // LSL = 0.65*USL = 0x82 @3.3V

  _i2c.Send_I2C_Byte(0x5A, ATO_CFGT, 0xB5);  // Target = 0.9*USL = 0xB5 @3.3V
  _i2c.Send_I2C_Byte(TOUCH_ADDR, ELE_CFG, 0x0C);
}

TouchSensor::~TouchSensor()
{
}

void TouchSensor::process()
{
  _i2c.Read_Multi_Byte(TOUCH_ADDR, 0x00, 2);
    
    LSB = i2cptr->I2C_RD_Buf[0];
    MSB = i2cptr->I2C_RD_Buf[1];
    
    //cout << "LSB " << hex << (int) LSB << endl;
    //cout << "MSB " << hex << (int) MSB << endl;

    unsigned short touched = ((MSB << 8) | LSB);


    for (int i=0; i < 12; i++){  // Check what electrodes were pressed
      if(touched & (1<<i)){

        if(touchStates[i] == 0){
          //pin i was just touched
          cout << "pin " << i << " was just touched" << endl;
        }else if(touchStates[i] == 1){
          //cout << "pin " << i << " is held" << endl;
        }

        touchStates[i] = 1;
      }else{
        if(touchStates[i] == 1){
          cout << "pin " << i << " was just released" << endl;

          //pin i is no longer being touched
        }

        touchStates[i] = 0;
      }

    }
 
}
