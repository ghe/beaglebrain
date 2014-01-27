#ifndef LOG_H
#define LOG_H

#include <stdio.h>
#include <stdarg.h>

class Log {
public:

  virtual ~Log() {
    if (_fp) {
      fclose(_fp);
    }
  }

  static void log(const char *fmt, ...) {
    Log &instance = Log::getInstance();
    va_list va;
    va_start(va, fmt);
    vprintf(fmt, va);
    instance.logToFile(fmt, va);
    va_end(va);
  }

private:

  static Log& getInstance()
  {
    static Log instance;
    return instance;
  }

  void logToFile(const char * fmt, va_list va) {
    if (_fp) {
      vfprintf(_fp, fmt, va);
    }
  }

  Log() {
     _fp = fopen("vipobrainlog", "w");
  }

  Log(const Log &);
  void operator=(const Log&);
  FILE *_fp;
};

#endif
