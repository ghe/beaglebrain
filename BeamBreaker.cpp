#include <fcntl.h>
#include <errno.h>

BeamBreaker::BeamBreaker()
{
  //echo 20 > /sys/class/gpio/export
  open("/sys/class/gpio/export"
//echo "in" > /sys/class/gpio/gpio20/direction
}


int BeamBreaker::update()
{
  return 0;
}

bool BeamBreaker::beamIsBroken()
{
  return _value < 1;
}
