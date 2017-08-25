#ifndef __CLASS_PRIV_H__
#define __CLASS_PRIV_H__

#include <object/class.h>
#include <object-priv.h>

#include <stdarg.h>
#include <stdlib.h>

#define VTABLE_INIT_SIZE	3

struct VTableEntry
{
  Selector key;
  Method item;

  struct VTableEntry *next;
};

struct VTableEntry *vtable_entry_create (Selector selector, Method method);
void vtable_entry_destroy (struct VTableEntry *self);

struct VTable
{
  struct VTableEntry **entries;
  unsigned int table_size;
  unsigned int table_count;
};

struct VTable *vtable_create (unsigned int _size);
void vtable_destroy(struct VTable *self);
struct VTable *vtable_cpy (struct VTable *_dest, struct VTable *src);

struct Class
{
  const struct Object _;

  const char *name;
  const struct Class *super;
  size_t size;

  struct VTable *vtable;

  void *         (*ctor) (void *_self, va_list *app);
  void *         (*dtor) (void *_self);
  bool           (*equals) (const void *_self, const void *o);
  struct String *(*to_string) (const void *_self);
  int            (*hash_code) (const void *_self);
};

void *class_constructor (void *_self, va_list *app);
void *class_destructor (void *_self);
struct String *class_to_string (const void *_self);

void class_register_method (const void *_self, Selector selector, Method method);

#endif /* __CLASS_PRIV_H__ */
