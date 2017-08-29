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
 * @file class.c
 * @author Jake Shilling
 * @brief Definitions for functions relating to the Class structure.
 */

#include <class-priv.h>

#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>
#include <string.h>
#include <math.h>

#include <jco/jco.h>

/*
 * STATIC HELPERS
 */

static unsigned int
get_index (struct VTable *self, Selector selector)
{
  unsigned int hash = (unsigned int) (unsigned long) selector;

  return hash % self->table_size;
}

static bool
is_prime (const unsigned int x)
{
  double lim = sqrt ((double) x);
  for (int i = 2; i <= lim; i++)
    if (x % i == 0)
      return false;

  return true;
}

static unsigned int
next_prime (const unsigned int x)
{
  if (x < 3)
    return 3;

  unsigned int ret = x % 2 == 0 ? x + 1 : x + 2;
  while (!is_prime (ret))
    ret += 2;

  return ret;
}

/*
 * MAIN INTERFACE
 */

size_t
jco_size_of (void const *const _self)
{
  const struct Class *const class =
    jco_is_descendant (_self, Class) ?
    jco_cast (_self, Class) : jco_class_of (_self);

  return class->size;
}

struct Class *
jco_super (void const *const _self)
{
  const struct Class *const class =
    jco_is_descendant (_self, Class) ?
    jco_cast (_self, Class) : jco_class_of (_self);

  return (struct Class *)class->super;
}

Method
jco_class_get_method (void const *const _self, Selector const selector)
{
  const struct Class *const class =
    jco_is_descendant (_self, Class) ?
    jco_cast (_self, Class) : jco_class_of (_self);

  if (selector == (Selector) jco_construct)
    return (Method) class->ctor;
  else if (selector == (Selector) jco_destruct)
    return (Method) class->dtor;
  else if (selector == (Selector) jco_equals)
    return (Method) class->equals;
  else if (selector == (Selector) jco_hash_code)
    return (Method) class->hash_code;
  else if (selector == (Selector) jco_to_string)
    return (Method) class->to_string;

  struct VTableEntry *bin = class->vtable->entries[get_index (class->vtable,
							      selector)];

  while (bin)
    if (bin->key == selector)
      return bin->item;
    else
      bin = bin->next;

  return NULL;
}

bool
jco_class_implements_method (void const *const _self, Selector const selector)
{
  return jco_class_get_method (_self, selector) != NULL;
}

bool
jco_class_implements_all (void const *const _self, ...)
{
  va_list args;
  va_start (args, _self);

  Selector selector = 0;
  while ((selector = va_arg(args, Selector)))
    if (!jco_class_implements_method (_self, selector))
      {
	va_end (args);
	return false;
      }

  va_end (args);
  return true;
}

/*
 * SELECTORS
 */


void *
jco_construct (void *_self, va_list *app)
{
  void *(*method)(void *, va_list *) = 0;
  DEFINE_SELECTOR (jco_construct, method, _self, app)
}

void *
jco_super_construct (struct Class const *const class, void *_self, va_list *app)
{
  jco_preconditions_check_not_null (class);
  jco_preconditions_check_not_null (class->super);
  jco_preconditions_check_not_null (class->super->ctor);

  return class->super->ctor (_self, app);
}

void *
jco_destruct (void *_self)
{
  void *(*method)(void *) = 0;
  DEFINE_SELECTOR (jco_destruct, method, _self)
}

void *
jco_super_destruct (struct Class const *const class, void *_self)
{
  jco_preconditions_check_not_null (class);
  jco_preconditions_check_not_null (class->super);
  jco_preconditions_check_not_null (class->super->dtor);

  return class->super->dtor (_self);
}

struct String *
jco_to_string (void const *const _self)
{
  initString ();
  struct String *(*method)(void const *const _self) = 0;
  DEFINE_SELECTOR (jco_to_string, method, _self)
}

bool
jco_equals (void const *const _self, void const *const o)
{
  bool (*method)(void const *const _self, void const *const o);
  DEFINE_SELECTOR (jco_equals, method, _self, o)
}

int
jco_hash_code (void const *const _self)
{
  int (*method)(void const *const) = 0;
  DEFINE_SELECTOR (jco_hash_code, method, _self)
}

/*
 * OVERRIDED METHODS
 */

void *
jco_class_constructor (void *_self, va_list *app)
{
  struct Class *self = _self;

  self->name = va_arg(*app, const char *);
  self->super = va_arg(*app, struct Class *);
  self->size = va_arg(*app, size_t);
  self->vtable = jco_vtable_cpy (NULL, self->super->vtable);

  if (NULL == self->super)
    {
      jco_log (SEVERE, "%O does not have a super class", self);
      exit (EXIT_FAILURE);
    }

  const size_t offset = offsetof(struct Class, ctor);
  memcpy ((char *) self + offset, (char *) self->super + offset,
	  jco_size_of (Class) - offset);

  Selector selector;
  while ((selector = va_arg(*app, Selector)))
    {
      Method method = va_arg(*app, Method);
      jco_class_register_method (self, selector, method);
    }

  return self;
}

void *
jco_class_destructor (void *_self)
{
  return NULL;
}

struct String *
jco_class_to_string (void const *const _self)
{
  struct Class *self = jco_cast (_self, Class);
  return jco_new (String, self->name);
}

void
jco_class_register_method (const void *_self, Selector selector, Method method)
{
  struct Class *self = jco_cast (_self, Class);

  if (selector == (Selector) jco_construct)
    *(Method *) &self->ctor = method;
  else if (selector == (Selector) jco_destruct)
    *(Method *) &self->dtor = method;
  else if (selector == (Selector) jco_equals)
    *(Method *) &self->equals = method;
  else if (selector == (Selector) jco_hash_code)
    *(Method *) &self->hash_code = method;
  else if (selector == (Selector) jco_to_string)
    *(Method *) &self->to_string = method;
  else
    {

      struct VTableEntry *entry = jco_vtable_entry_create (selector, method);

      if (self->vtable == NULL)
	self->vtable = jco_vtable_create (VTABLE_INIT_SIZE);
      struct VTableEntry *bin = self->vtable->entries[get_index (self->vtable,
								 selector)];
      if (!bin)
	{
	  self->vtable->entries[get_index (self->vtable, selector)] = entry;
	  self->vtable->table_count++;
	}
      else
	{
	  while (bin->next)
	    bin = bin->next;

	  bin->next = entry;
	  self->vtable->table_count++;
	}

      if (self->vtable->table_count >= self->vtable->table_size)
	{
	  struct VTable *vtable = jco_vtable_create (
	      next_prime (self->vtable->table_size));

	  jco_vtable_cpy (vtable, self->vtable);

	  jco_vtable_destroy (self->vtable);
	  self->vtable = vtable;
	}
    }
}

/**
 * VTABLE
 */

struct VTableEntry *
jco_vtable_entry_create (Selector selector, Method method)
{
  struct VTableEntry *ret = jco_malloc (sizeof(struct VTableEntry));
  ret->key = selector;
  ret->item = method;
  ret->next = NULL;

  return ret;
}

void
jco_vtable_entry_destroy (struct VTableEntry *self)
{
  if (self->next)
    jco_vtable_entry_destroy (self->next);

  jco_free (self);
}

struct VTable *
jco_vtable_create (unsigned int _size)
{
  unsigned int size = is_prime (_size) ? _size : next_prime (_size);

  struct VTable *ret = jco_malloc (sizeof(struct VTable));
  ret->table_count = 0;
  ret->table_size = size;
  ret->entries = jco_calloc (size, sizeof(struct VTableEntry));

  return ret;
}

void
jco_vtable_destroy (struct VTable *self)
{
  for (int i = 0; i < self->table_size; i++)
    if (self->entries[i])
      jco_vtable_entry_destroy (self->entries[i]);

  jco_free (self);
}

struct VTable *
jco_vtable_cpy (struct VTable *_dest, struct VTable *src)
{
  if (src == NULL)
    return NULL;

  struct VTable *dest = _dest == NULL ? jco_vtable_create (src->table_size) : _dest;

  for (int i = 0; i < src->table_size; i++)
    {
      if (src->entries[i])
	{
	  struct VTableEntry *bin = src->entries[i];

	  while (bin)
	    {
	      struct VTableEntry *newbin = dest->entries[get_index (dest,
								    bin->key)];
	      if (!newbin)
		{
		  dest->entries[get_index (dest, bin->key)] =
		      jco_vtable_entry_create (bin->key, bin->item);
		  dest->table_count++;
		}
	      else
		{
		  while (newbin->next)
		    newbin = newbin->next;

		  newbin->next = jco_vtable_entry_create (bin->key, bin->item);
		  dest->table_count++;
		}

	      bin = bin->next;
	    }
	}
    }

  return dest;
}
