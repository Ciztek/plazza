#include "Logger.hpp"

LogSettings LogStream::SETTINGS;

LogStream::LogStream(LogLevel level, const char *file, int line)
  : _level(level), _file(file), _line(line)
{
}

LogStream::~LogStream()
{
  flush();
}

void LogStream::flush()
{
  if (_level < SETTINGS.filter)
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

  if (SETTINGS.type == LogType::JSON) {
    std::fprintf(
      SETTINGS.output,
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
  } else {
    std::fprintf(
      SETTINGS.output,
      "[%s] %s (%s:%d): %s\n",
      timebuf,
      level_str,
      _file,
      _line,
      _ss.str().c_str());
  }

  std::fflush(SETTINGS.output);
}

auto LogStream::log(LogLevel lvl, const char *file, int line) -> LogStream
{
  return {lvl, file, line};
}

void LogStream::
  logger_configure(const std::string &log_path, LogLevel level, LogType type)
{
  LogStream::SETTINGS.filter = level;
  LogStream::SETTINGS.type = type;

  if (!log_path.empty()) {
    FILE *fp = std::fopen(log_path.c_str(), "w");
    if (!fp) {
      std::perror("Failed to open log file");
      std::exit(1);
    }
  }
}
