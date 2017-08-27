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

const size_t
size_of (const void *_self)
{
  const struct Class *class = jco_cast (_self, Class);
  return class->size;
}
const struct Class *
super (const void *_self)
{
  const struct Class *class = jco_cast (_self, Class);
  return class->super;
}

Method
class_get_method (const void *_self, Selector selector)
{
  const struct Class *class = jco_is_descendant (_self, Class) ?
    jco_cast (_self, Class) : jco_class_of (_self);

  if (selector == (Selector) construct)
    return (Method) class->ctor;
  else if (selector == (Selector) destruct)
    return (Method) class->dtor;
  else if (selector == (Selector) equals)
    return (Method) class->equals;
  else if (selector == (Selector) hash_code)
    return (Method) class->hash_code;
  else if (selector == (Selector) to_string)
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
class_implements_method (const void *_self, Selector selector)
{
  return class_get_method (_self, selector) != NULL;
}

bool
class_implements_all (const void *_self, ...)
{
  va_list args;
  va_start (args, _self);

  Selector selector = 0;
  while ((selector = va_arg(args, Selector)))
    if (!class_implements_method (_self, selector))
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
construct (void *_self, va_list *app)
{
  void *(*method)(void *, va_list *) = 0;
  DEFINE_SELECTOR (construct, method, _self, app)
}

void *
super_construct (const struct Class *class, void *_self, va_list *app)
{
  jco_preconditions_check_not_null (class);
  jco_preconditions_check_not_null (class->super);
  jco_preconditions_check_not_null (class->super->ctor);

  return class->super->ctor (_self, app);
}

void *
destruct (void *_self)
{
  void *(*method)(void *) = 0;
  DEFINE_SELECTOR (destruct, method, _self)
}

void *
super_destruct (const struct Class *class, void *_self)
{
  jco_preconditions_check_not_null (class);
  jco_preconditions_check_not_null (class->super);
  jco_preconditions_check_not_null (class->super->dtor);

  return class->super->dtor (_self);
}

const struct String *
to_string (const void *_self)
{
  initString ();
  const struct String *(*method)(const void *_self) = 0;
  DEFINE_SELECTOR (to_string, method, _self)
}

bool
equals (const void *_self, const void *o)
{
  bool (*method)(const void *_self, const void *o) = 0;
  DEFINE_SELECTOR (equals, method, _self, o)
}

int
hash_code (const void *_self)
{
  int (*method)(const void *) = 0;
  DEFINE_SELECTOR (hash_code, method, _self)
}

/*
 * OVERRIDED METHODS
 */

void *
class_constructor (void *_self, va_list *app)
{
  struct Class *self = _self;

  self->name = va_arg(*app, const char *);
  self->super = va_arg(*app, struct Class *);
  self->size = va_arg(*app, size_t);
  self->vtable = vtable_cpy (NULL, self->super->vtable);

  if (NULL == self->super)
    {
      jco_log (SEVERE, "%O does not have a super class", self);
      exit (EXIT_FAILURE);
    }

  const size_t offset = offsetof(struct Class, ctor);
  memcpy ((char *) self + offset, (char *) self->super + offset,
	  size_of (Class) - offset);

  Selector selector;
  while ((selector = va_arg(*app, Selector)))
    {
      Method method = va_arg(*app, Method);
      class_register_method (self, selector, method);
    }

  return self;
}

void *
class_destructor (void *_self)
{
  return NULL;
}

struct String *
class_to_string (const void *_self)
{
  struct Class *self = jco_cast (_self, Class);
  return jco_new (String, self->name);
}

void
class_register_method (const void *_self, Selector selector, Method method)
{
  struct Class *self = jco_cast (_self, Class);

  if (selector == (Selector) construct)
    *(Method *) &self->ctor = method;
  else if (selector == (Selector) destruct)
    *(Method *) &self->dtor = method;
  else if (selector == (Selector) equals)
    *(Method *) &self->equals = method;
  else if (selector == (Selector) hash_code)
    *(Method *) &self->hash_code = method;
  else if (selector == (Selector) to_string)
    *(Method *) &self->to_string = method;
  else
    {

      struct VTableEntry *entry = vtable_entry_create (selector, method);

      if (self->vtable == NULL)
	self->vtable = vtable_create (VTABLE_INIT_SIZE);
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
	  struct VTable *vtable = vtable_create (
	      next_prime (self->vtable->table_size));

	  vtable_cpy (vtable, self->vtable);

	  vtable_destroy (self->vtable);
	  self->vtable = vtable;
	}
    }
}

/**
 * VTABLE
 */

struct VTableEntry *
vtable_entry_create (Selector selector, Method method)
{
  struct VTableEntry *ret = jco_malloc (sizeof(struct VTableEntry));
  ret->key = selector;
  ret->item = method;
  ret->next = NULL;

  return ret;
}

void
vtable_entry_destroy (struct VTableEntry *self)
{
  if (self->next)
    vtable_entry_destroy (self->next);

  jco_free (self);
}

struct VTable *
vtable_create (unsigned int _size)
{
  unsigned int size = is_prime (_size) ? _size : next_prime (_size);

  struct VTable *ret = jco_malloc (sizeof(struct VTable));
  ret->table_count = 0;
  ret->table_size = size;
  ret->entries = jco_calloc (size, sizeof(struct VTableEntry));

  return ret;
}

void
vtable_destroy (struct VTable *self)
{
  for (int i = 0; i < self->table_size; i++)
    if (self->entries[i])
      vtable_entry_destroy (self->entries[i]);

  jco_free (self);
}

struct VTable *
vtable_cpy (struct VTable *_dest, struct VTable *src)
{
  if (src == NULL)
    return NULL;

  struct VTable *dest = _dest == NULL ? vtable_create (src->table_size) : _dest;

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
		      vtable_entry_create (bin->key, bin->item);
		  dest->table_count++;
		}
	      else
		{
		  while (newbin->next)
		    newbin = newbin->next;

		  newbin->next = vtable_entry_create (bin->key, bin->item);
		  dest->table_count++;
		}

	      bin = bin->next;
	    }
	}
    }

  return dest;
}
