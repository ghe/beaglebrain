#ifndef COMMAND_H
#define COMMAND_H

class Command
{
public:
  typedef enum _Type {
    None = 0x00,
    Ping = 0x01,
    SensorUpdate = 0x02,
    Servo = 0x10,
    SetServoPos = 0x11,
    SetServoPosPct = 0x12,
    SetServoLimitMin = 0x13,
    SetServoLimitMax = 0x14,
    SetServoMapping = 0x15
    PlaySound = 0x31,
    PlayMotion = 0x32,
    PlayCombo = 0x33
  } Type;
  Type type() const;
  bool parse(const unsigned char *buffer);
  unsigned int data(unsigned int index) const { return _data[index]; }

protected:
  Type _type;
  unsigned int _data[2];
};

#endif //COMMAND_H
