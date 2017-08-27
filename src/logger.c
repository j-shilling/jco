#include <stdio.h>
#include <stdbool.h>
#include <time.h>
#include <errno.h>
#include <string.h>
#include <stdarg.h>

#include <jco/jco.h>
#include <jco/util/parsefmt.h>

static struct
{
  bool print_to_term;
  bool print_to_file;

  const char *path_to_error_log;
  const char *path_to_warning_log;
  const char *path_to_info_log;
  const char *path_to_config_log;
  const char *path_to_fine_log;

  enum LogLevel level;
  enum LogLevel default_level;
} logger =
  {
  true, false, NULL, NULL, NULL, NULL, NULL, INFO, FINE };

struct LogLevelInfo
{
  const char * const string;

  FILE ** const term_file;
  const char **log_file;
};

static FILE *out = NULL;
static FILE *err = NULL;

static const struct LogLevelInfo levels_table[] =
  {
    { "FINEST", &out, &logger.path_to_fine_log },
    { "FINER", &out, &logger.path_to_fine_log },
    { "FINE", &out, &logger.path_to_fine_log },
    { "CONFIG", &out, &logger.path_to_config_log },
    { "INFO", &out, &logger.path_to_info_log },
    { "WARNING", &err, &logger.path_to_warning_log },
    { "SEVERE", &err, &logger.path_to_error_log } };

static void
logger_log_cstring (enum LogLevel level, const char * const msg)
{
  if (out == NULL)
    out = stdout;
  if (err == NULL)
    err = stderr;

  if (level < logger.level)
    return;

  const struct LogLevelInfo level_info = levels_table[level];

  time_t ltime = time (NULL);
  char *timestamp = ctime(&ltime);
  timestamp[strlen (timestamp) - 1] = '\0';

  const struct String *log = jco_new (String, timestamp);
  string_append (log, ": [");
  string_append (log, (char *)level_info.string);
  string_append (log, "] ");
  string_append (log, (char *)msg);

  if (!string_ends_with (log, "\n"))
  string_append (log, "\n");

  if (logger.print_to_term)
    fprintf (*level_info.term_file, "%s", string_to_cstring (log));

  if (logger.print_to_file)
    {
      int index = level;
      const char *filename = *level_info.log_file;
      while (index > 0 && NULL == filename)
	{
	  index--;
	  filename = *levels_table[index].log_file;
	}
      if (NULL == filename)
	{
	  logger.print_to_file = false;
	  logger_log_cstring (WARNING, "No log file is defined.");
	  logger.print_to_file = true;
	  return;
	}

      FILE *logfile = fopen (filename, "a");
      if (NULL == logfile)
	{
	  logger.print_to_file = false;

	  int msg_size = snprintf (NULL, 0, "Could not open %s; %s.",
				   *level_info.log_file, strerror (errno));
	  char *file_err_msg = calloc (msg_size + 1, sizeof(char));
	  sprintf (file_err_msg, "Could not open %s; %s.", *level_info.log_file,
		   strerror (errno));

	  logger_log_cstring (SEVERE, file_err_msg);
	  logger.print_to_file = true;
	}

      if (0 > fprintf (logfile, "%s", string_to_cstring (log)))
	{
	  logger.print_to_file = false;

	  int msg_size = snprintf (NULL, 0, "Could not write to %s.",
				   *level_info.log_file);
	  char *file_err_msg = calloc (msg_size + 1, sizeof(char));
	  sprintf (file_err_msg, "Could not write to %s.",
		   *level_info.log_file);

	  logger_log_cstring (SEVERE, file_err_msg);
	  logger.print_to_file = true;
	}

      if (NULL != logfile)
	{
	  if (0 != fclose (logfile))
	    {
	      logger.print_to_file = false;

	      int msg_size = snprintf (NULL, 0, "Could not close %s; %s.",
				       *level_info.log_file, strerror (errno));
	      char *file_err_msg = calloc (msg_size + 1, sizeof(char));
	      sprintf (file_err_msg, "Could not close %s; %s.",
		       *level_info.log_file, strerror (errno));

	      logger_log_cstring (SEVERE, file_err_msg);
	      logger.print_to_file = true;
	    }
	}
    }
}

static void
jco_vlog (enum LogLevel const level,
	  char const *const restrict fmt,
	  va_list ap)
{
  char *msg = NULL;
  parsefmt (&msg, fmt, ap);
  logger_log_cstring (level, msg);
  free (msg);
}

void
jco_logger_log (enum LogLevel const level,
	    char const *const restrict fmt,
	    ...)
{
  va_list args;
  va_start(args, fmt);

  jco_vlog (level, fmt, args);

  va_end (args);
}

void
jco_logger_log_default_level (char const *const restrict fmt,
			      ...)
{
  va_list args;
  va_start(args, fmt);

  jco_vlog (logger.default_level, fmt, args);

  va_end (args);
}
