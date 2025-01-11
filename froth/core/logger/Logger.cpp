#include "Logger.h"

#include <cstdarg>
#include <cstdio>
#include <cstring>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <type_traits>

namespace Froth {

const char *levelStrings[6] = {"[FATAL]: ", "[ERROR]: ", "[WARN]: ", "[INFO]: ", "[DEBUG]: ", "[TRACE]: "};
void Logger::log(LogLevel level, const char *message, ...) {
  const int MAX_LOG_SIZE = 32000;
  char outMessage[32000];
  char outMessage2[32000];
  memset(outMessage, 0, MAX_LOG_SIZE);

  va_list arg_ptr;
  va_start(arg_ptr, message);
  vsnprintf(outMessage, MAX_LOG_SIZE, message, arg_ptr);
  va_end(arg_ptr);

  std::snprintf(outMessage2, MAX_LOG_SIZE, "%s%s\n", levelStrings[static_cast<std::underlying_type<LogLevel>::type>(level)], outMessage);
  printf("%s", outMessage2);
}

} // namespace Froth
