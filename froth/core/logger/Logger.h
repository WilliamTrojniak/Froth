#pragma once

#define FROTH_WARN_ENABLED 1
#define FROTH_INFO_ENABLED 1
#define FROTH_DEBUG_ENABLED 1
#define FROTH_TRACE_ENABLED 1

namespace Froth {
class Logger {
public:
  enum class LogLevel {
    FATAL = 0,
    ERROR = 1,
    WARN = 2,
    INFO = 3,
    DEBUG = 4,
    TRACE = 5,
  };

  static void log(LogLevel level, const char *message, ...);
};

#ifndef FROTH_FATAL
#define FROTH_FATAL(message, ...) Logger::log(Logger::LogLevel::FATAL, message, __VA_ARGS__);
#endif // !FROTH_FATAL

#ifndef FROTH_ERROR
#define FROTH_ERROR(message, ...) Logger::log(Logger::LogLevel::ERROR, message, __VA_ARGS__);
#endif // !FROTH_ERROR

#if FROTH_WARN_ENABLED == 1
#define FROTH_WARN(message, ...) Logger::log(Logger::LogLevel::WARN, message, __VA_ARGS__);
#else
#define FROTH_WARN(message, ...)
#endif // !FROTH_WARN_ENABLED == 1

#if FROTH_INFO_ENABLED == 1
#define FROTH_INFO(message, ...) Logger::log(Logger::LogLevel::INFO, message, ##__VA_ARGS__);
#else
#define FROTH_INFO(message, ...)
#endif // !FROTH_WARN_ENABLED == 1

#if FROTH_DEBUG_ENABLED == 1
#define FROTH_DEBUG(message, ...) Logger::log(Logger::LogLevel::DEBUG, message, __VA_ARGS__);
#else
#define FROTH_DEBUG(message, ...)
#endif // !FROTH_DEBUG_ENABLED == 1

#if FROTH_TRACE_ENABLED == 1
#define FROTH_TRACE(message, ...) Logger::log(Logger::LogLevel::TRACE, message, __VA_ARGS__);
#else
#define FROTH_TRACE(message, ...)
#endif // !FROTH_TRACE_ENABLED == 1
} // namespace Froth
