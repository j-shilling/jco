#include <jco/api/string.h>
#include <string-priv.h>

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdio.h>

#include <jco/jco.h>
#include <jco/util/parsefmt.h>

const struct Class * String = NULL;

void
initString()
{
  if (NULL == String)
    String = jco_new (Class, "String", Object, sizeof(struct String),
	construct, string_constructor,
	destruct, string_destructor,
	equals, string_equals,
	hash_code, string_hash_code,
	to_string, string_to_string,
	0);
}

const char *
string_to_cstring (const void *_self)
{
  struct String *self = jco_cast (_self, String);
  return self->text;
}

void
string_append_char (const void *_self, char ch)
{
  struct String *self = jco_cast (_self, String);

  int len = string_length (self) + 1;
  char *newtext = jco_calloc (len + 1, sizeof (char));
  strcpy (newtext, self->text);
  newtext[len - 1] = ch;

  jco_free (self->text);
  self->text = newtext;
}

void
string_append_cstring (const void *_self, char *str)
{
  struct String *self = jco_cast (_self, String);
  if (NULL == str)
    str = "";

  int len = snprintf (0, 0, "%s%s", self->text, str);
  char *newtext = jco_calloc (len + 1, sizeof (char));
  sprintf (newtext, "%s%s", self->text, str);

  if (self->text)
    {
      free ((void *)self->text);
    }
  self->text = newtext;
}

void
string_append_object (const void *_self, void *o)
{
  const struct String *self = jco_cast (_self, String);
  const struct String *str = to_string (o);
  string_append_cstring (self, (char *)string_to_cstring (str));
  jco_unref (str);
}

bool
string_ends_with_char (const void *_self, char ch)
{
  return string_char_at (_self, string_length (_self) - 1) == ch;
}
bool
string_ends_with_cstring (const void *_self, char *str)
{
  if (NULL == str)
    {
      jco_log (WARNING, "Testing to see if a string ends with NULL.");
      return false;
    }

  struct String *self = jco_cast (_self, String);
  int mylen = string_length (self);
  int yourlen = strlen (str);
  if (yourlen > mylen)
    return false;

  int i = mylen - 1; int j = yourlen - 1;
  while (i >= 0 && j >= 0)
    {
      if (str[j] != self->text[i])
	return false;

      i--; j--;
    }

  return true;
}

bool
string_ends_with_object (const void *_self, void *o)
{
  return string_ends_with_cstring (_self,
				   (char *)string_to_cstring (to_string (o)));
}

unsigned int
string_length(const void *_self)
{
  struct String *self = jco_cast (_self, String);
  return strlen (self->text);
}

char
string_char_at (const void *_self, unsigned int index)
{
  const struct String *self = jco_cast (_self, String);

  if (index >= string_length (self))
    return '\0';

  return self->text[index];
}

void *
string_constructor (void *_self, va_list *app)
{
  struct String *self = _self;

  const char *fmt = va_arg (*app, const char *);
  char *buf = NULL;
  parsefmt (&buf, fmt ? fmt : "", *app);

  self->text = jco_calloc (strlen (buf) + 1, sizeof (char));
  strcpy ((char *)self->text, buf);
  free (buf);

  return self;
}

void *
string_destructor (void *_self)
{
  struct String *self = jco_cast (_self, String);

  if (self->text)
    jco_free ((void *)self->text);

  return self;
}

bool
string_equals (const void *_self, const void *o)
{
  if (NULL == o)
    return _self == NULL;

  struct String *self = jco_cast (_self, String);

  return 0 == strcmp (self->text,
		      jco_is_descendant (o, String) ?
			  ((struct String *)o)->text :
			  (const char *) o);
}

struct String *
string_to_string (const void *_self)
{
  return jco_cast (_self, String);
}

int
string_hash_code (const void *_self)
{
  int h = 0;
  const char *cstr = string_to_cstring (_self);
  int len = strlen (cstr);

  for (int i = 0; i < len; i ++)
    h = 31 * h + (int)(*(cstr++));

  return h;
}
