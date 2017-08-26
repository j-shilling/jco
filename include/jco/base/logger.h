#ifndef INCLUDE_OBJECT_LOGGER_H_
#define INCLUDE_OBJECT_LOGGER_H_

#include <jco/api/string.h>

#define logger_log(x, y) _Generic ((y), \
				    char *: logger_log_cstring, \
			      const char *: logger_log_cstring, \
		         const char *const: logger_log_cstring, \
			   struct String *: logger_log_string, \
		     const struct String *: logger_log_string, \
		const struct String *const: logger_log_string) (x, y)

enum LogLevel
{
  FINEST,
  FINER,
  FINE,
  CONFIG,
  INFO,
  WARNING,
  SEVERE,

  N_LEVELS
};

void logger_log_cstring (enum LogLevel level, const char *const msg);
void logger_log_string (enum LogLevel level, const struct String *const msg);
void logger_logf (enum LogLevel level, const char *fmt, ...);

#endif /* INCLUDE_OBJECT_LOGGER_H_ */
