#include "Robot.h"
#include "Command.h"
#include "Message.h"

Robot::Robot()  :
_running(false)
,_incoming(5000)
,_outgoing(5001)
,_ping(false)
,_sensorUpdate(false)
{}

int Robot::run()
{
  _running = true;
  while (_running) {
    processState();
    //check command socket
    processCommands();
    processFeedback();
    usleep(1000);
  }
	return 0;
}

void Robot::stop()
{
  _running = false;
}

void Robot::processState()
{
  _mediaPlayer.process();
  _touchSensor.process();
}

void Robot::processCommands()
{
  _incoming.checkClients();
	Command cmd;
  if (_incoming.commandReceived(&cmd)) {
    switch(cmd.type()) {
      case Command::Ping:
        _ping = true;
        break;
      case Command::SensorUpdate:
        _sensorUpdate = true;
        break;
      case Command::SetServoPos:
        _servoController.setPosition(cmd.data(0), cmd.data(1));
        break;
      case Command::SetServoPosPct:
        _servoController.setPositionPct(cmd.data(0), cmd.data(1));
        break;
      case Command::SetServoMapping:
        _servoController.setChannelMapping(cmd.data(0), cmd.data(1));
        break;
      case Command::SetServoLimitMin:
        _servoController.setChannelLimitMin(cmd.data(0), cmd.data(1));
        break;
      case Command::SetServoLimitMax:
        _servoController.setChannelLimitMax(cmd.data(0), cmd.data(1));
        break;
      case Command::PlaySound:
        _mediaPlayer.PlaySound(cmd.data(0));
        break;
      case Command::PlayMotion:
        _mediaPlayer.PlayMotion(cmd.data(0));
        break;
      case Command::PlayCombo:
        _mediaPlayer.PlayCombo(cmd.data(0), cmd.data(1));
        break;
      default:
        break;
    }
  }
}

void Robot::processFeedback()
{
  _outgoing.checkClients();
  if (_ping) {
    PongMessage msg;
    if (_outgoing.publishMessage(&msg)) {
			_ping = false;
		}
  }
  if (_sensorUpdate) {
    SensorMessage msg;
    fillSensorMessage(&msg);
    if (_outgoing.publishMessage(&msg)) {
			_sensorUpdate = false;
		}
  }
}

void Robot::fillSensorMessage(SensorMessage *msg)
{

}
