#include <jco/util/parsefmt.h>
#include <jco/util/asprintf.h>
#include <jco/jco.h>

#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdio.h>

struct buf
{
  char *buf;
  size_t len;
  size_t size;
};

static void
add_char_to_buf (struct buf *restrict buf, char const ch)
{
  if (buf->len >= buf->size)
    {
      buf->buf = jco_realloc (buf->buf, buf->size + 10);
      buf->size += 10;
    }

  buf->buf[buf->len] = ch;
  buf->len ++;
}

static void
add_jco_string_to_buf (struct buf *restrict buf,
		   char const *const restrict str)
{
  for (int i = 0; i < strlen (str); i ++)
    add_char_to_buf (buf, str[i]);
}

size_t
parsefmt (char **const restrict buf,
      char const *const restrict fmt,
      va_list ap)
{
  struct buf b = { jco_calloc (10, 1), 0, 10 };

  for (int i = 0; i < strlen (fmt); i++)
    {
      if (fmt[i] == '%')
	{
	  if ((i + 1) >= strlen (fmt))
	    {
	      add_char_to_buf (&b, fmt[i]);
	    }
	  else if (fmt[i + 1] == 'O')
	    {
	      void *str = jco_to_string(va_arg (ap, void *));
	      add_jco_string_to_buf (&b, jco_string_to_cstring (str));
	      jco_unref (str);

	      i++;
	    }
	  else
	    {
	      struct buf s = { jco_calloc (10, 1), 0, 10 };
	      add_char_to_buf (&s, fmt[i]);
	      for (int j = i+1; j < strlen (fmt); j++)
		{
		  add_char_to_buf (&s, fmt[j]);
		  if (isspace (fmt[j])
		      || fmt[j] == 'd'
		      || fmt[j] == 'i'
		      || fmt[j] == 'o'
		      || fmt[j] == 'u'
		      || fmt[j] == 'x'
		      || fmt[j] == 'X'
		      || fmt[j] == 'e'
		      || fmt[j] == 'E'
		      || fmt[j] == 'f'
		      || fmt[j] == 'F'
		      || fmt[j] == 'g'
		      || fmt[j] == 'G'
		      || fmt[j] == 'a'
		      || fmt[j] == 'c'
		      || fmt[j] == 's'
		      || fmt[j] == 'C'
		      || fmt[j] == 'S'
		      || fmt[j] == 'p'
		      || fmt[j] == 'n'
		      || fmt[j] == 'm'
		      || fmt[j] == '%')
		    {
		      break;
		    }
		}

	      i += strlen (s.buf)-1;

	      va_list temp;
	      va_copy (temp, ap);

	      char *sl = jco_malloc (vsnprintf (0, 0, s.buf, temp) + 1);
	      va_end (temp);

	      vsprintf (sl, s.buf, ap);

	      add_jco_string_to_buf (&b, sl);

	      jco_free (sl);
	    }
	}
      else
	{
	  add_char_to_buf (&b, fmt[i]);
	}
    }

  *buf = jco_calloc (b.len + 1, 1);
  strcpy (*buf, b.buf);
  jco_free (b.buf);
}
