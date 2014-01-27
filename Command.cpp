#include "Command.h"
#include "Log.h"

Command::Type Command::type() const
{
  return _type;
}

bool Command::parse(const unsigned char *buffer)
{
  _type = (Type)(buffer[0] | (buffer[1] << 8));
  _data[0] = buffer[2] | (buffer[3] << 8);
  _data[1] = buffer[4] | (buffer[5] << 8);
  Log::log("command parse type %02X data %02X %02X\n", _type, _data[0], _data[1]);
  return true;
}

