#ifndef MESSAGE_H
#define MESSAGE_H
class Message
{
public:
  virtual int length() const = 0;
  virtual const unsigned char* toBytes() = 0;
  virtual ~Message() {}
};

class PongMessage : public Message
{
public:
  int length() const { return 4; }
  const unsigned char *toBytes() { return (unsigned char *)"pong"; }
};

class SensorMessage : public Message
{
public:
  int length() const { return 4; }
  const unsigned char *toBytes() { return (unsigned char *)"pong"; }
};

#endif //MESSAGE_H

