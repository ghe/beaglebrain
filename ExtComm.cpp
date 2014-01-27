#include "ExtComm.h"
#include <stdio.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <errno.h>
#include "Log.h"

ExtComm::ExtComm(int port)
{
  _sd = socket(AF_INET, SOCK_STREAM, 0);
  if (_sd >= 0) {
    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family      = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port        = htons(port);

    int result = bind(_sd, (struct sockaddr *) &addr, sizeof(addr));
    if (result >= 0) {
      result = listen(_sd, SOMAXCONN);
      if (result >= 0) {
        int on = 1;
        result = setsockopt(_sd, SOL_SOCKET,  SO_REUSEADDR, (void *)&on, sizeof(on));
        if (result >= 0) {
          result = ioctl(_sd, FIONBIO, (void *)&on);
          if (result >= 0) {
            FD_ZERO(&_fds);
            FD_SET(_sd, &_fds);
            _max_sd = _sd;
            Log::log("socket %d ready\n", port);
            return;
          } else Log::log("FAIL: set non blocking\n");
        } else Log::log("FAIL: set reusable\n");
      } else Log::log("FAIL: listen\n");
    } else Log::log("FAIL: bind\n");
  } else Log::log("FAIL: socket\n");
  close(_sd);
  _sd = -1;
}

ExtComm::~ExtComm()
{
  if (_sd >= 0) {
    close(_sd);
    Log::log("closing socket: %d\n", _sd);
  }
}

int ExtComm::checkClients()
{
  int connections_accepted = 0;
  if (_sd >= 0) {
    struct timeval timeout;
    timeout.tv_sec = 0;
    timeout.tv_usec = 100;
    fd_set tmp_fds;
    memcpy(&tmp_fds, &_fds, sizeof(_fds));
    int result = select(_max_sd + 1, &tmp_fds, NULL, NULL, &timeout);
    //someone is ready
    if (result >= 0) {
      int waiting = result;
      for (int i=0; i <= _max_sd && waiting > 0; ++i) {
        //check each one
        if (FD_ISSET(i, &tmp_fds)) {
          waiting--;
          //if it is the listening socket, accept the connection
          if (i == _sd) {
            int sd = 0;
            while (sd >= 0) {
              sd = accept(_sd, NULL, NULL);
              if (sd < 0) {
                if (errno == EWOULDBLOCK) {
                  break;
                }
                else {
                  Log::log("FAIL: accept\n");
                  return false;
                }
              }
              //we have accepted a new connection
              //add it to the readable set
              connections_accepted++;
              Log::log("connection %d accepted\n", sd);
              int on =1;
              result = ioctl(sd, FIONBIO, (void *)&on);
              FD_SET(sd, &_fds);
              _max_sd = (sd > _max_sd) ? sd : _max_sd;
            }
          }
        }
      }
    }
  }
  return connections_accepted;
}

int ExtComm::commandReceived(Command *cmd)
{
  if (_sd >= 0) {
    struct timeval timeout;
    timeout.tv_sec = 0;
    timeout.tv_usec = 100;
    fd_set tmp_fds;
    memcpy(&tmp_fds, &_fds, sizeof(_fds));
    int result = select(_max_sd + 1, &tmp_fds, NULL, NULL, &timeout);
    //someone is ready
    if (result >= 0) {
      int waiting = result;
      for (int i=0; i <= _max_sd && waiting > 0; ++i) {
        //check each one
        if (FD_ISSET(i, &tmp_fds)) {
          waiting--;
          //if it is not the listening socket, read from it
          if (i != _sd) {
            //an existing connection became readable
            if (readHeader(i)) {
              unsigned char buffer[6] = {0};
              if (readCmd(i, buffer, sizeof(buffer))) {
                //don't read more than one command, and let them pile up in the network buffer if there is too much
                cmd->parse(buffer);
                Log::log("valid command\n");
                return 1;
              } else Log::log("invalid command\n");
            }
          }
        }
      }
    }
  }
  return 0;
}

//this is to be robust against garbage
bool ExtComm::readHeader(int sd)
{
  const unsigned char header[2] = {0xF1, 0xD0};
  unsigned char c = 0;
  for (size_t i=0; i<sizeof(header);) {
    int len = recv(sd, &c, 1, 0);
    if (len < 1) {
      if (len == 0 || errno != EWOULDBLOCK) {
        closeConnection(sd);
      }
      return false;
    }
    //if we find the next header character
    //advance, but reset and keep looking on mismatch
    if (c == header[i]) {
      i++;
    } else {
      i=0;
    }
  }
  return true;
}

bool ExtComm::readCmd(int sd, unsigned char *buffer, size_t buflen)
{
  int len = recv(sd, buffer, buflen, 0);
  if (len < 1) {
    if (len == 0 || errno != EWOULDBLOCK) {
      closeConnection(sd);
    }
    return false;
  }
  return true;
}

int ExtComm::publishMessage(Message *msg)
{
  int messages_sent = 0;
  if (_sd >= 0) {
    struct timeval timeout;
    timeout.tv_sec = 0;
    timeout.tv_usec = 100;
    fd_set tmp_fds;
    memcpy(&tmp_fds, &_fds, sizeof(_fds));
    int result = select(_max_sd + 1, NULL, &tmp_fds, NULL, &timeout);
    //someone is ready
    if (result >= 0) {
      int waiting = result;
      for (int i=0; i <= _max_sd && waiting > 0; ++i) {
        //check each one
        if (FD_ISSET(i, &tmp_fds)) {
          waiting--;
          //if it is the listening socket, accept the connection
          if (i != _sd) {
            //an existing connection is writeable
            int len = send(i, msg->toBytes(), msg->length(), 0);
            if (len != msg->length()) {
              perror("FAIL: send\n");
              closeConnection(i);
            }
            else {
              messages_sent++;
            }
          }
        }
      }
    }
  }
  return messages_sent;
}

bool ExtComm::closeConnection(int sd)
{
  close(sd);
  Log::log("connection %d closed\n", sd);
  FD_CLR(sd, &_fds);
  if (sd == _max_sd) {
    while (!FD_ISSET(_max_sd, &_fds)) {
      _max_sd--;
    }
  }
  return true;
}
