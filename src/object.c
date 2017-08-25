#include <object/object.h>
#include <object-priv.h>

#include <stdlib.h>
#include <stdio.h>

#include <object/api/string.h>

void *
new (const struct Class *class, ...)
{
  if (NULL == class)
    return NULL;

  struct Object *object;
  va_list ap;

  object = calloc (1, size_of (class));
  if (NULL == object)
    return NULL;

  object->class = class;
  object->magic_number = MAGIC_NUMBER;

  va_start (ap, class);
  object = construct (object, &ap);
  va_end (ap);

  return ref (object);
}

void
delete (void *_self)
{
  if (_self)
    free (destruct (_self));
}

const struct Class *
class_of (const void *_self)
{
  if (NULL == _self || !is_object (_self))
    return NULL;

  const struct Object *self = _self;
  return self->class;
}

bool
is_object (const void *_self)
{
  return _self && ((struct Object *)_self)->magic_number == MAGIC_NUMBER;
}

bool
is_descendant (const void *_self, const struct Class *class)
{
  if (is_object (_self) && NULL != class)
    {
      const struct Object *self = _self;
      const struct Class *myclass = class_of (self);

      if (class != Object)
        while (myclass != class)
	  if (myclass != Object)
            myclass = super (myclass);
          else
            return false;

      return true;
    }

  return false;
}

bool
is (const void *_self, const struct Class *class)
{
  return is_object (_self) && class_of (_self) == class;
}

void *
cast (const void *_self, const struct Class *class)
{
  if (NULL == _self)
    {;
      logger_log (SEVERE, "Cannot cast a null object.");
      exit (EXIT_FAILURE);
    }
  if (NULL == class)
    {
      logger_log (SEVERE, "Cannot cast an object to a null class.");
      exit (EXIT_FAILURE);
    }
  if (!is_descendant (_self, class))
    {
      const struct String *msg = new (String, "Cannot cast a ");
      string_append (msg, unref(to_string (class_of (_self))));
      string_append (msg, " to a ");
      string_append (msg, unref(to_string (class)));

      logger_log (SEVERE, msg);

      exit (EXIT_FAILURE);
    }

  return (void *)_self;
}

void *
ref (const void *_self)
{
  return (void *)_self;
}

void *
unref (const void *_self)
{
  return (void *)_self;
}

void *
object_constructor (void *_self, va_list *app)
{
  return _self;
}

void *
object_destructor (void *_self)
{
  return _self;
}

bool
object_equals (const void *_self, const void *o)
{
  return _self == o;
}

struct String *
object_to_string (const void *_self)
{
  struct Object *self = cast (_self, Object);
  const struct String *classname = to_string (class_of (self));

  int len = snprintf (0, 0, "%s@%p",
		  string_to_cstring (classname),
		  self);

  char *buf = calloc (1, len + 1);
  sprintf (buf, "%s@%p",
		  string_to_cstring (classname),
		  self);
  struct String *ret = new (String, buf);
  free (buf);
  unref (classname);

  return ret;
}

int
object_hash_code (const void *_self)
{
  return (int) (long) _self;
}
