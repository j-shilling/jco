#include <stdio.h>
#include <stdbool.h>
#include <time.h>
#include <errno.h>
#include <string.h>
#include <stdarg.h>

#include <jco/jco.h>
#include <jco/util/parsefmt.h>
#include <jco/util/apsrintf.h>

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
  if (level < logger.level)
    return;

  if (out == NULL)
    out = stdout;
  if (err == NULL)
    err = stderr;

  const struct LogLevelInfo level_info = levels_table[level];

  time_t ltime = time (NULL);
  char *timestamp = ctime(&ltime);
  timestamp[strlen (timestamp) - 1] = '\0';

  char *log = NULL;
  char *file_err_msg = NULL;

  {
    int i = asprintf (&log, "%s: [%s] %s\n", timestamp, level_info.string, msg);
    if (log[i - 2] == '\n')
      log[i - 1] = '\0';
  }

  if (logger.print_to_term)
    fprintf (*level_info.term_file, "%s", log);

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

	  char *file_err_msg = NULL;
	  asprintf (&file_err_msg, "Could not open %s; %s.", *level_info.log_file,
		   strerror (errno));

	  logger_log_cstring (SEVERE, file_err_msg);
	  logger.print_to_file = true;

	  goto cleanup;
	}

      if (0 > fprintf (logfile, "%s", string_to_cstring (log)))
	{
	  logger.print_to_file = false;

	  char *file_err_msg = NULL;
	  asprintf (&file_err_msg, "Could not write to %s.",
		   *level_info.log_file);

	  logger_log_cstring (SEVERE, file_err_msg);
	  logger.print_to_file = true;

	  goto cleanup;
	}

      if (NULL != logfile)
	{
	  if (0 != fclose (logfile))
	    {
	      logger.print_to_file = false;

	      char *file_err_msg = NULL;
	      asprintf (&file_err_msg, "Could not close %s; %s.",
		       *level_info.log_file, strerror (errno));

	      logger_log_cstring (SEVERE, file_err_msg);
	      logger.print_to_file = true;

	      goto cleanup;
	    }
	}
    }

cleanup:
  if (log) free (log);
  if (file_err_msg) free (file_err_msg);

  return;
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
