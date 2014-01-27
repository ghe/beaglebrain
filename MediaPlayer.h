#ifndef MEDIAPLAYER_H
#define MEDIAPLAYER_H

#include <unistd.h>
#include <string>
#include <map>

class MediaPlayer
{
public:
  MediaPlayer();
  virtual ~MediaPlayer();

  bool playSound(int id);
  bool playMotion(int id);
  bool playCombo(int idSound, int idMotion);
protected:
  std::map<int, std::string> _mediaPaths;
};
#endif // MEIAPLAYER_H
