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
 * @file object-priv.h
 * @author Jake Shilling
 * @brief A header file for the private definition of Object
 */
#ifndef __OBJECT_PRIV_H__
#define __OBJECT_PRIV_H__

#define MAGIC_NUMBER	0x0effaced

#include <stdbool.h>
#include <stdarg.h>

/**
 * @brief Represents an object instance
 */
struct Object
{
  unsigned int magic_number; /** Identifies this as an object */
  const struct Class *class; /** Points to this object's type information */
};

void *jco_object_constructor (void *_self, va_list *app);
void *jco_object_destructor (void *_self);
bool jco_object_equals (const void *_self, const void *o);
struct String *jco_object_to_string (const void *_self);
int jco_object_hash_code (const void *_self);

#endif /* __OBJECT_PRIV_H__ */
