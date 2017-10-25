/*
* This file is part of JCO
*
* JCO is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* JCO is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with JCO.  If not, see <http://www.gnu.org/licenses/>.
*/

/**
 * @file object.c
 * @author Jake Shilling
 * @brief Implement functions for working with instances of Object
 */

#include <object-priv.h>

#include <stdlib.h>
#include <stdio.h>

#include <jco/jco.h>

void *
jco_new (struct Class const *const class, ...)
{
  jco_preconditions_check_not_null (class);

  struct Object *object;
  va_list ap;

  object = jco_calloc (1, jco_size_of (class));

  object->class = class;
  object->magic_number = MAGIC_NUMBER;

  va_start (ap, class);
  object = jco_construct (object, &ap);
  va_end (ap);

  return jco_ref (object);
}

void
jco_delete (void *const _self)
{
  if (_self)
    jco_free (jco_destruct (_self));
}

const struct Class *
jco_class_of (void const *const _self)
{
  jco_preconditions_check_not_null (_self);
  jco_preconditions_check_arg (jco_is_object (_self));

  const struct Object *self = _self;
  return self->class;
}

bool
jco_is_object (void const *const _self)
{
  return _self && ((struct Object *)_self)->magic_number == MAGIC_NUMBER;
}

bool
jco_is_descendant (void const *const _self, struct Class const *const class)
{
  jco_preconditions_check_not_null (class);

  if (jco_is_object (_self))
    {
      const struct Class *myclass = NULL;

      if (class != Class && jco_is_descendant (_self, Class))
	{
	  myclass = jco_cast (_self, Class);
	}
      else
	{
	  myclass = jco_class_of (_self);
	}

      if (class != Object)
        while (myclass != class)
	  if (myclass != Object)
            myclass = jco_super (myclass);
          else
            return false;

      return true;
    }

  return false;
}

bool
jco_is (void const *const _self, struct Class const *const class)
{
  jco_preconditions_check_not_null (class);
  
  return jco_is_object (_self) && jco_class_of (_self) == class;
}

void *
jco_cast (void const *const _self, struct Class const *const class)
{
  jco_preconditions_check_arg (jco_is_object (_self));
  jco_preconditions_check_not_null (class);
  jco_preconditions_check_arg_msg (jco_is_descendant (_self, class),
				   "Cannot cast a %O to a %O.",
				   jco_class_of (_self),
				   class);
  return (void *)_self;
}

void *
jco_ref (void const *const _self)
{
  return (void *)_self; // for now this does nothing
}

void *
jco_unref (void const *const _self)
{
  return (void *)_self; // for now this does nothing
}

void *
jco_object_constructor (void *_self, va_list *app)
{
  return _self;
}

void *
jco_object_destructor (void *_self)
{
  return _self;
}

bool
jco_object_equals (const void *_self, const void *o)
{
  return _self == o;
}

struct String *
jco_object_to_string (const void *_self)
{
  struct Object *self = jco_cast (_self, Object);

  struct String *classname = jco_to_string (jco_class_of (self));

  int len = snprintf (0, 0, "%s@%p",
		  jco_string_to_cstring (classname),
		  self);

  char *buf = jco_calloc (1, len + 1);
  sprintf (buf, "%s@%p",
		  jco_string_to_cstring (classname),
		  self);
  struct String *ret = jco_new (String, buf);
  jco_free (buf);
  jco_unref (classname);

  return ret;
}

int
jco_object_hash_code (const void *_self)
{
  return jco_hash_object (_self);
}
