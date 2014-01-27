#include "MediaPlayer.h"
#include <signal.h>

#ifdef ARCH
  #define SOUND_PLAYER "mplg123"
#else
  #define SOUND_PLAYER "mplayer"
#endif

MediaPlayer::MediaPlayer()
{
  _mediaPaths[0] = "/vipo/media/001 new friend/1A.mp3";
}

virtual MediaPlayer::~MediaPlayer()
{
}

bool MediaPlayer::playSound(int id)
{
  if (_soundPid > 0) {
    kill(_soundPid, SIGKILL);
    _soundPid = 0;
  }
  char *command[3] = { SOUND_PLAYER, 0, 0 };
  command[1] = getMediaPath(id);
  if (command[1]) {
    _soundPid = forkExec(command);
    return (_soundPid > 0);
  }
  return false;
}

bool MediaPlayer::playMotion(int id)
{
  
}

bool MediaPlayer::playCombo(int idSound, int idMotion)
{
}

char *MediaPlayer::getMediaPath(int id)
{
  if (_mediaPaths.find(id) != _mediaPaths.end()) {
    return _mediaPaths[id].c_str();
  }
  return NULL;
}

pid_t MediaPlayer::forkExec(const char **cmd)
{
  pid = fork();

  if (pid < 0) {
    Log::log("ERROR: MediaPlayer::forkExec fork %d\n", pid);
    return pid;
  } else {
    if (pid == 0) {
      execvp(*command, command);
      Log::log("ERROR: MediaPlayer::forkExec execvp %d\n", errno);
      exit(1);
    }
  }

  Log::log("INFO: MediaPlayer::forkExec forked %d\n", pid);
  return pid;
}

