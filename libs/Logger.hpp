#pragma once

#include <cstdio>
#include <cstring>
#include <ctime>
#include <sstream>

enum class LogLevel {
  DEBUG,
  INFO,
  WARN,
  CRIT
};

class LogStream {
public:
  static LogLevel FILTER;

  LogStream(LogLevel level, const char *file, int line)
    : _level(level), _file(file), _line(line)
  {
  }

  ~LogStream()
  {
    flush();
  }

  template <typename T> auto operator<<(const T &val) -> LogStream &
  {
    _ss << val;
    return *this;
  }

  void flush()
  {
    if (_level < FILTER)
      return;

    char timebuf[sizeof "YYYY-MM-DDTHH:MM:SSZ"];
    std::time_t now = std::time(nullptr);
    std::strftime(timebuf, sizeof timebuf, "%FT%TZ", std::gmtime(&now));

    const char *level_str;
    switch (_level) {
      case LogLevel::DEBUG:
        level_str = "DEBUG";
        break;
      case LogLevel::INFO:
        level_str = "INFO";
        break;
      case LogLevel::WARN:
        level_str = "WARN";
        break;
      case LogLevel::CRIT:
        level_str = "CRITICAL";
        break;
      default:
        __builtin_unreachable();
    }

    std::fprintf(
      stdout,  // TODO: Consider `-f` for log file
      "{"
      "\"timestamp\":\"%s\","
      "\"level\":\"%s\","
      "\"file\":\"%s\","
      "\"line\":%d,"
      "\"message\":\"%s\""
      "}\n",
      timebuf,
      level_str,
      _file,
      _line,
      _ss.str().c_str());
  }

  static auto log(LogLevel lvl, const char *file, int line) -> LogStream
  {
    return {lvl, file, line};
  }

private:
  LogLevel _level;
  const char *_file;
  int _line;
  std::ostringstream _ss;
};

class Log : public LogStream {};

#define debug log(LogLevel::DEBUG, __FILE__, __LINE__)
#define info log(LogLevel::INFO, __FILE__, __LINE__)
#define warn log(LogLevel::WARN, __FILE__, __LINE__)
#define failed log(LogLevel::CRIT, __FILE__, __LINE__)
