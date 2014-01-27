#include "ServoController.h"
#include "Log.h"

#define PWM_ADDR    0x40
#define MODE1_REG   0x00
#define PRESCALE_REG  0xFE
#define CH_1_REG    0x06

#define SERVO_MIN   200
#define SERVO_MAX   400

#define MAX_CHANNELS 16

const unsigned char ServoController::_channelRegisters[] = {
  0x06, 0x0A, 0x0E, 0x12,
  0x16, 0x1A, 0x1E, 0x22,
  0x26, 0x2A, 0x2E, 0x32,
  0x36, 0x3A, 0x3E, 0x42,
};

ServoController::ServoController(const I2CInterface &i2c) :
_i2c(i2c)
{
  Log::log("ServoController()\n");
  unsigned char currentMode;

  Log::log("reset the chip\n");
  _i2c.Send_I2C_Byte(PWM_ADDR, MODE1_REG, 0x00);

  Log::log("read the current mode\n");
  currentMode = _i2c.Read_I2C_Byte(PWM_ADDR, MODE1_REG);
  _i2c.Read_I2C_Byte(PWM_ADDR, 0x01);

  Log::log("put the chip to sleep so we can change the frequency\n");
  _i2c.Send_I2C_Byte(PWM_ADDR, MODE1_REG, (currentMode & 0x7f)| 0x10);

  Log::log("set the frequency prescale to 50 Hz -> 0x80\n");
  _i2c.Send_I2C_Byte(PWM_ADDR, PRESCALE_REG, 0x80);

  Log::log("give the chip time to change its clock\n");
  usleep(5000);

  Log::log("set auto register increment capability and wake up chip\n");
  _i2c.Send_I2C_Byte(PWM_ADDR, MODE1_REG, currentMode | 0xA1);

}

ServoController::~ServoController()
{
}

void ServoController::setChannelLimitMin(unsigned int channel, unsigned int value)
{
  Log::log("setChannelLimitMin channel %d, min %d\n", channel, value);
  _channelLimits[channel].min = value;
}

void ServoController::setChannelLimitMax(unsigned int channel, unsigned int value)
{
  Log::log("setChannelLimitMax channel %d, max %d\n", channel, value);
  _channelLimits[channel].max = value;
}

void ServoController::setChannelMapping(unsigned int aliasChannel, unsigned int hwChannel)
{
  Log::log("setChannelMapping alias %d for channel\n", aliasChannel, hwChannel);
  _channelMappings[aliasChannel] = hwChannel;
}

bool ServoController::setPosition(unsigned int channel, unsigned int posUs)
{
  Log::log("setPosition channel %d, pos %d\n", channel, posUs);
  unsigned char channelNum = channel;
  //if there is a mapping, use it
  if (_channelMappings.find(channel) != _channelMappings.end()) {
    channelNum = _channelMappings[channel];
    Log::log("setPosition mapped channel %d to %d\n", channel, channelNum);
  }

  if (channelNum < MAX_CHANNELS) {

    unsigned char channelRegister = _channelRegisters[channel];

    //use the mapped channel for the limits
    if (_channelLimits.find(channel) != _channelLimits.end()) {
      posUs = std::min(_channelLimits[channel].max, posUs);
      posUs = std::max(_channelLimits[channel].min, posUs);
    }

    //12 bits = 4096. 50Hz = 20ms
    unsigned int pwmPos = (4096*posUs) / 20000;

    unsigned char pwmValue[4];
    //on-time is always the same
    pwmValue[0] = 0;
    pwmValue[1] = 0;

    //servo position
    pwmValue[2] = pwmPos;
    pwmValue[3] = pwmPos >> 8;

    Log::log("setPosition channelRegister %02X pwm %d\n", channelRegister, pwmPos);
    _i2c.Send_Multi_Byte(PWM_ADDR, channelRegister, pwmValue, 4);
    return true;
  }
  Log::log("setPosition invalid channel %d\n", channelNum);
  return false;
}

bool ServoController::setPositionPct(int channel, double posPct)
{
  Log::log("setPositionPct channel %d, pct %f\n", channel, posPct);
  //12 bits = 4096. 50Hz = 20ms
  //12 bits @ 50 Hz for 1ms to 2ms = 204.8 -> 409.6 ::
  //307.6 = 1.5 ms
  //unsigned int center = 308;
  unsigned int min = 1000;
  unsigned int max = 2000;
  //use the mapped channel for the limits
  if (_channelLimits.find(channel) != _channelLimits.end()) {
    min = _channelLimits[channel].min;
    max = _channelLimits[channel].max;
  }
  unsigned int posUs = min+((posPct*(max-min))/100);
  return setPosition(channel, posUs);
}
