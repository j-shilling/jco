#include <stdio.h>
#include <stdbool.h>
#include <time.h>
#include <errno.h>
#include <string.h>
#include <stdarg.h>

#include <jco/jco.h>

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
} logger =
  {
  true, false, NULL, NULL, NULL, NULL, NULL, INFO };

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
init_streams ()
{
  if (out == NULL)
    out = stdout;
  if (err == NULL)
    err = stderr;
}

void
logger_log_cstring (enum LogLevel level, const char * const msg)
{
  init_streams ();
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

void
logger_log_string (enum LogLevel level, const struct String * const msg)
{
  logger_log_cstring (level, string_to_cstring (msg));
}

static char *
get_buffer (char *buf, unsigned int size)
{
  char *newbuf = jco_calloc (size, sizeof(char));
  if (NULL != buf)
    {
      snprintf (newbuf, size, "%s", buf);
      free (buf);
    }
  return newbuf;
}

void
logger_logf (enum LogLevel level, const char *fmt, ...)
{
  va_list args;
  va_start(args, fmt);

  static const unsigned int INIT_BUF_SIZE = 100;
  char *buf = get_buffer (NULL, INIT_BUF_SIZE);
  int buf_size = INIT_BUF_SIZE;

  int index = 0;
  for (int i = 0; i < strlen (fmt); i++)
    {
      if (fmt[i] == '%')
	{
	  if ((i + 1) < strlen (fmt) && fmt[i + 1] == 'O')
	    {
	      void *obj = va_arg(args, void *);
	      const struct String *str = to_string (obj);

	      for (int j = 0; j < string_length (str); j++)
		{
		  if (index > buf_size)
		    buf = get_buffer (buf, buf_size + INIT_BUF_SIZE);

		  buf[index] = string_char_at (str, j);
		  index++;
		}

	      jco_unref (str);
	    }
	  else
	    {
	      struct String *str = jco_new (String, "");
	      while (i < strlen (fmt) && fmt[i] != ' ')
		{
		  string_append (str, fmt[i]);
		  i++;
		}

	      int size = snprintf (0, 0, "%s%s", buf, string_to_cstring (str));
	      char *fmt_arg = calloc (size, sizeof(char));
	      sprintf (fmt_arg, "%s%s", buf, string_to_cstring (str));

	      if (string_ends_with (str, 's'))
		{
		  char *x = va_arg(args, char *);
		  int n = snprintf (0, 0, fmt_arg, x);
		  if (n > buf_size)
		    buf = get_buffer (buf, n);

		  sprintf (buf, fmt_arg, x);
		}
	      else if (string_ends_with (str, 'd') || string_ends_with (str, 'i')
		  || string_ends_with (str, 'c'))
		{
		  int x = va_arg(args, int);
		  int n = snprintf (0, 0, fmt_arg, x);
		  if (n > buf_size)
		    buf = get_buffer (buf, n);

		  sprintf (buf, fmt_arg, x);
		}
	      else if (string_ends_with (str, 'o') || string_ends_with (str, 'x')
	      || string_ends_with (str, 'X') || string_ends_with (str, 'u'))
		{
		  unsigned int x = va_arg(args, unsigned int);
		  int n = snprintf (0, 0, fmt_arg, x);
		  if (n > buf_size)
		    buf = get_buffer (buf, n);

		  sprintf (buf, fmt_arg, x);
		}
	      else if (string_ends_with (str, 'f') || string_ends_with (str, 'F')
	      || string_ends_with (str, 'e') || string_ends_with (str, 'E')
	      || string_ends_with (str, 'a') || string_ends_with (str, 'A')
	      || string_ends_with (str, 'g') || string_ends_with (str, 'G'))
		{
		  double x = va_arg(args, double);
		  int n = snprintf (0, 0, fmt_arg, x);
		  if (n > buf_size)
		    buf = get_buffer (buf, n);

		  sprintf (buf, fmt_arg, x);
		}
	      else if (string_ends_with (str, 'p'))
		{
		  void *x = va_arg(args, void *);
		  int n = snprintf (0, 0, fmt_arg, x);
		  if (n > buf_size)
		    buf = get_buffer (buf, n);

		  sprintf (buf, fmt_arg, x);
		}
	      else if (string_ends_with (str, 'n'))
		{
		  int *x = va_arg(args, int *);
		  int n = snprintf (0, 0, fmt_arg, x);
		  if (n > buf_size)
		    buf = get_buffer (buf, n);

		  sprintf (buf, fmt_arg, x);
		}
	      else if (string_ends_with (str, '%'))
		{
		  if (index > buf_size)
		    buf = get_buffer (buf, buf_size + INIT_BUF_SIZE);
		  buf[index] = '%';
		}

	    }
	}
      else
	{
	  if (index > buf_size)
	    buf = get_buffer (buf, buf_size + INIT_BUF_SIZE);
	  buf[index] = fmt[i];
	}
    }

  logger_log_cstring (level, buf);
  free (buf);
}
