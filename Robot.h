#ifndef ROBOT_H
#define ROBOT_H

#include "ExtComm.h"
#include "ServoController.h"

class Robot
{
public:
  static Robot& getInstance()
  {
    static Robot instance;
    return instance;
  }
  int run();
  void stop();
protected:
  void processState();
  void processCommands();
  void processFeedback();
  void fillSensorMessage(SensorMessage *msg);

  bool _running;
	ExtComm _incoming;
	ExtComm _outgoing;
  ServoController _servoController;
  TouchSensor _touchSensor;
  MediaPlayer _mediaPlayer;

  bool _ping;
  bool _sensorUpdate;
  /*
  BeamBreaker _beamBreaker;
  TouchSensor touchSensors[12];
  Accelerometer accelerometer;
  Photocell photocell;
  AudioController audio;
  */
private:
  Robot();
  Robot(const Robot &);
  void operator=(const Robot&);
};

#endif //ROBOT_H
