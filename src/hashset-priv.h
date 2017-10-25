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

#ifndef __HASHSET_PRIV_H__
#define __HASHSET_PRIV_H__

#include <object-priv.h>

#include <stdbool.h>
#include <stddef.h>

#define MIN_RATIO	.25
#define MAX_RATIO	1

struct ImmutableHashSet
{
  struct Object const _;
  struct Class const *type;

  void **arr;
  unsigned int nmemb;
  size_t size;
};

struct HashSet
{
  struct ImmutableHashSet _;
};

void *jco_immutable_hash_set_constructor (void *_self, va_list *ap);
void *jco_immutable_hash_set_destructor (void *_self);

struct Class *jco_hash_set_content_type (void const *const _self);
void *jco_hash_set_iterator (void const *const _self);
unsigned int jco_hash_set_size (void const *const _self);
bool jco_hash_set_contains (void const *const _self, void const *const o);

bool jco_hash_set_add (void const *const _self, void const *const o);
bool jco_hash_set_remove (void const *const _self, void const *const o);

#endif /* __HASHSET_PRIV_H__ */
