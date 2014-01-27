#include <stdio.h>
#include <signal.h>

#include "Robot.h"

void sighandler(int param)
{
  printf("exit\n");
  Robot::getInstance().stop();
}

int main (int argc, char *argv[])
{
  //signal(SIGABRT, &sighandler);
  signal(SIGTERM, &sighandler);
  signal(SIGINT, &sighandler);
  return Robot::getInstance().run();
}
