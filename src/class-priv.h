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

#ifndef __CLASS_PRIV_H__
#define __CLASS_PRIV_H__

#include <jco/jco.h>
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

struct VTableEntry *jco_vtable_entry_create (Selector selector, Method method);
void jco_vtable_entry_destroy (struct VTableEntry *self);

struct VTable
{
  struct VTableEntry **entries;
  unsigned int table_size;
  unsigned int table_count;
};

struct VTable *jco_vtable_create (unsigned int _size);
void jco_vtable_destroy(struct VTable *self);
struct VTable *jco_vtable_cpy (struct VTable *_dest, struct VTable *src);

struct Class
{
  const struct Object _;

  const char *name;
  const struct Class *super;
  size_t size;

  struct VTable *vtable;

  void *         (*ctor) (void *_self, va_list *app);
  void *         (*dtor) (void *_self);
  bool           (*equals) (void const* const _self, void const *const o);
  struct String *(*to_string) (void const *const _self);
  int            (*hash_code) (void const *const _self);
};

void *jco_class_constructor (void *_self, va_list *app);
void *jco_class_destructor (void *_self);
struct String *jco_class_to_string (void const *const _self);

void jco_class_register_method (const void *_self, Selector selector, Method method);

#endif /* __CLASS_PRIV_H__ */
