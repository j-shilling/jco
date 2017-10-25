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
jco_init_string()
{
  if (NULL == String)
    String = jco_new (Class, "String", Object, sizeof(struct String),
	jco_construct, jco_string_constructor,
	jco_destruct, jco_string_destructor,
	jco_equals, jco_string_equals,
	jco_hash_code, jco_string_hash_code,
	jco_to_string, jco_string_to_string,
	0);
}

const char *
jco_string_to_cstring (void const *const _self)
{
  struct String *self = jco_cast (_self, String);
  return self->text;
}

void
jco_string_append_char (void const *const _self, char const ch)
{
  struct String *self = jco_cast (_self, String);

  int len = jco_string_length (self) + 1;
  char *newtext = jco_calloc (len + 1, sizeof (char));
  strcpy (newtext, self->text);
  newtext[len - 1] = ch;

  jco_free (self->text);
  self->text = newtext;
}

void
jco_string_append_cstring (void const *const _self, char const *const _str)
{
  struct String *self = jco_cast (_self, String);
  char const *const str = _str ? _str : "";

  int len = snprintf (0, 0, "%s%s", self->text, str);
  char *newtext = jco_calloc (len + 1, sizeof (char));
  sprintf (newtext, "%s%s", self->text, str);

  if (self->text)
    free ((void *)self->text);

  self->text = newtext;
}

void
jco_string_append_object (void const *const _self, void const *const o)
{
  const struct String *self = jco_cast (_self, String);
  const struct String *str = jco_to_string (o);
  jco_string_append_cstring (self, (char *)jco_string_to_cstring (str));
  jco_unref (str);
}

bool
jco_string_ends_with_char (void const *const _self, char const ch)
{
  return jco_string_char_at (_self, jco_string_length (_self) - 1) == ch;
}
bool
jco_string_ends_with_cstring (void const *const _self, char const *const str)
{
  if (NULL == str)
    {
      jco_log (WARNING, "Testing to see if a string ends with NULL.");
      return false;
    }

  struct String *self = jco_cast (_self, String);
  int mylen = jco_string_length (self);
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
jco_string_ends_with_object (void const *const _self, void const *const o)
{
  return jco_string_ends_with_cstring (_self,
				   (char *)jco_string_to_cstring (jco_to_string (o)));
}

unsigned int
jco_string_length(void const *const _self)
{
  struct String *self = jco_cast (_self, String);
  return strlen (self->text);
}

char
jco_string_char_at (void const *const _self, unsigned int const index)
{
  const struct String *self = jco_cast (_self, String);

  if (index >= jco_string_length (self))
    return '\0';

  return self->text[index];
}

void *
jco_string_constructor (void *_self, va_list *app)
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
jco_string_destructor (void *_self)
{
  struct String *self = jco_cast (_self, String);

  if (self->text)
    jco_free ((void *)self->text);

  return self;
}

bool
jco_string_equals (void const *const _self, void const *const o)
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
jco_string_to_string (void const *const _self)
{
  return jco_cast (_self, String);
}

int
jco_string_hash_code (void const *const _self)
{
  return jco_hash_string (jco_string_to_cstring (_self));
}
