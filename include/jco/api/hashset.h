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

#ifndef __HASHSET_H__
#define __HASHSET_H__

#include <jco/api/collection.h>
#include <jco/api/mutablecollection.h>
#include <jco/api/immutablearraylist.h>

struct HashSet;
extern struct Class const *HashSet;

struct ImmutableHashSet;
extern struct Class const *ImmutableHashSet;

void jco_init_hash_set();

#endif /* __HASHSET_H__ */
