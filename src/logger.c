#include <stdio.h>
#include <stdbool.h>
#include <time.h>
#include <errno.h>
#include <string.h>
#include <stdarg.h>

#include <jco/jco.h>
#include <jco/util/parsefmt.h>
#include <jco/util/asprintf.h>

/* Global Logging Settings */
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
} logger = {

/* Default Settings */
  true, /* Do print to stdout / stderr */
  false, /* Don't print to a file */

  NULL, /* No default file paths */
  NULL, /* No default file paths */
  NULL, /* No default file paths */
  NULL, /* No default file paths */
  NULL, /* No default file paths */

  INFO, /* Only log INFO or higher */
  FINE /* Default logs to FINE */
};

/* Stores info about how to handle a log level */
struct LogLevelInfo
{
  const char * const string; /* Convert level to a string */

  FILE ** const term_file; /* stdout or stderr */
  const char **log_file; /* the address to look for a log file */
};

static FILE *out = NULL;
static FILE *err = NULL;

/* Properties of each log level */
static const struct LogLevelInfo levels_table[] =
  {
    { "FINEST", &out, &logger.path_to_fine_log },
    { "FINER", &out, &logger.path_to_fine_log },
    { "FINE", &out, &logger.path_to_fine_log },
    { "CONFIG", &out, &logger.path_to_config_log },
    { "INFO", &out, &logger.path_to_info_log },
    { "WARNING", &err, &logger.path_to_warning_log },
    { "SEVERE", &err, &logger.path_to_error_log }
  };

static void
logger_log_cstring (enum LogLevel level, const char * const msg)
{
  // Filter out low priority levels
  if (level < logger.level)
    return;

  // Initialize out and err
  if (out == NULL)
    out = stdout;
  if (err == NULL)
    err = stderr;

  // Get info for current log level
  const struct LogLevelInfo level_info = levels_table[level];

  // Set up time stamp
  time_t ltime = time (NULL);
  char *timestamp = ctime(&ltime);
  timestamp[strlen (timestamp) - 1] = '\0'; // remove trailing \n

  // Messages to be logged
  char *log = NULL;
  char *file_err_msg = NULL;

  // Set log message and ensure it ends with a \n
  {
    int i = asprintf (&log, "%s: [%s] %s\n", timestamp, level_info.string, msg);
    if (log[i - 2] == '\n')
      log[i - 1] = '\0';
  }

  // Display in term
  if (logger.print_to_term)
    fprintf (*level_info.term_file, "%s", log);

  // Log to file maybe
  if (logger.print_to_file)
    {
      // If no file is set for this log level, see if one of the
      // lower levels has one
      int index = level;
      const char *filename = *level_info.log_file;
      while (index > 0 && NULL == filename)
	{
	  index--;
	  filename = *levels_table[index].log_file;
	}
      if (NULL == filename)
	{
	  // No file path set
	  logger.print_to_file = false;
	  logger_log_cstring (WARNING, "No log file is defined.");
	  logger.print_to_file = true;
	  return;
	}

      FILE *logfile = fopen (filename, "a");
      if (NULL == logfile)
	{
	  // File could not be opened
	  logger.print_to_file = false;

	  char *file_err_msg = NULL;
	  asprintf (&file_err_msg, "Could not open %s; %s.", *level_info.log_file,
		   strerror (errno));

	  logger_log_cstring (SEVERE, file_err_msg);
	  logger.print_to_file = true;

	  goto cleanup;
	}

      if (0 > fprintf (logfile, "%s", log))
	{
	  // Write error
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
	      // Close error
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

void
jco_log_to_file (bool const val)
{
  logger.print_to_file = val;
}

void
jco_log_to_term (bool const val)
{
  logger.print_to_term = val;
}

void
jco_log_level (enum LogLevel const level)
{
  logger.level = level;
}

void
jco_default_log_level (enum LogLevel const level)
{
  logger.default_level = level;
}

void
jco_log_file (enum LogLevel const level, char const *const path)
{
  *levels_table[level].log_file = path;
}
