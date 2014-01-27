#ifndef TOUCHSENSOR_H
#define TOUCHSENSOR_H

#include "I2CInterface.h"

class TouchSensor
{
public:
  TouchSensor();
  virtual ~TouchSensor();
private:
	I2CInterface _i2c;

};

#endif //TOUCHSENSOR_H
