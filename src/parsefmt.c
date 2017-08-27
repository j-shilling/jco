#include <jco/util/parsefmt.h>
#include <jco/util/apsrintf.h>
#include <jco/jco.h>

#include <string.h>
#include <stdlib.h>

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
add_string_to_buf (struct buf *restrict buf,
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

  for (int i = 0; i < strlen (fmt) - 1; i++)
    {
      if (fmt[i] == '%')
	{
	  if (fmt[i + 1] == 'O')
	    {
	      void *str = to_string(va_arg (ap, void *));
	      add_string_to_buf (&b, string_to_cstring (str));
	      jco_unref (str);
	    }
	  else
	    {
	      struct buf s = { jco_calloc (10, 1), 0, 10 };
	      for (int j = i+1; j < strlen (fmt) && j != ' '; j++)
		add_char_to_buf (&s, fmt[j]);

	      char *sl = NULL;
	      asprintf (&sl, s.buf, ap);
	      add_string_to_buf (&b, sl);
	      free (sl);
	    }
	}
      else
	{
	  add_char_to_buf (&b, fmt[i]);
	}
    }
  add_char_to_buf (&b, fmt[strlen(fmt)+1]);

  *buf = jco_malloc (b.len);
  strcpy (*buf, b.buf);
  jco_free (b.buf);
}
