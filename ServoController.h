#ifndef SERVOCONTROLLER_H
#define SERVOCONTROLLER_H
#include "I2CInterface.h"
#include <map>

class ServoController
{
protected:
  static const unsigned char _channelRegisters[];
  typedef struct _Limit {
	  unsigned int min;
		unsigned int max;
	} Limit;
public:
	ServoController(const I2CInterface &i2c);
	virtual ~ServoController();
	void setChannelLimitMin(unsigned int channel, unsigned int value);
	void setChannelLimitMax(unsigned int channel, unsigned int value);
	void setChannelMapping(unsigned int aliasChannel, unsigned int hwChannel);
	bool setPositionPct(int channel, double posPct);
	bool setPosition(unsigned int channel, unsigned int posUs);
protected:
	const I2CInterface &_i2c;
  std::map<unsigned int, unsigned int> _channelMappings;
  std::map<unsigned int, Limit> _channelLimits;
};

#endif // SERVOCONTROLLER_H

