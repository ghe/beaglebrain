#ifndef EXTCOMM_H
#define EXTCOMM_H

#include <sys/socket.h>
#include "Command.h"
#include "Message.h"

class ExtComm
{
public:
  ExtComm(int port);
  virtual ~ExtComm();
  int checkClients();
  int commandReceived(Command *cmd);
  int publishMessage(Message *msg);
protected:
  bool readHeader(int sd);
  bool readCmd(int sd, unsigned char *buffer, size_t len);
	bool closeConnection(int sd);
	int _sd;
	int _max_sd;
	fd_set _fds;
};

#endif //EXTCOMM_H
