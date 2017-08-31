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
 * @file string.h
 * @author Jake Shilling
 * @brief prototypes for functions which deal with the String class
 */

#ifndef INCLUDE_OBJECT_API_STRING_H_
#define INCLUDE_OBJECT_API_STRING_H_

#include <stdbool.h>

/** _Generic Macro which calls the appropriate jco_string_append_*
  function */
#define jco_string_append(x, y) _Generic ((y),  \
		   int: jco_string_append_char, \
		  char: jco_string_append_char, \
	     char *: jco_string_append_cstring, \
       const char *: jco_string_append_cstring, \
  const char *const: jco_string_append_cstring, \
      default: jco_string_append_object) (x, y)

/** _Generic Macro which calls the appropriate jco_string_ends_with_*
  function */
#define jco_string_ends_with(x, y) _Generic ((y),  \
		   int: jco_string_ends_with_char, \
		  char: jco_string_ends_with_char, \
	     char *: jco_string_ends_with_cstring, \
       const char *: jco_string_ends_with_cstring, \
  const char *const: jco_string_ends_with_cstring, \
	default: jco_string_ends_with_object) (x, y)

struct String; /** Structure to represent and instance of String */
extern const struct Class * String; /** Type information for the String
				      class */

/** Initialize the global String variable. Must be called
  before any instances of String are created */
void jco_init_string();

const char *jco_string_to_cstring (void const *const _self);
void jco_string_append_cstring (void const *const _self, char const *const str);
void jco_string_append_object (void const *const _self, void const *const o);
void jco_string_append_char (void const *const _self, char const ch);
unsigned int jco_string_length(void const *const _self);
bool jco_string_ends_with_char (void const *const _self, char const ch);
bool jco_string_ends_with_cstring (void const *const _self, char const *const str);
bool jco_string_ends_with_object (void const *const _self, void const *const o);
char jco_string_char_at (void const *const _self, unsigned int const index);

#endif /* INCLUDE_OBJECT_API_STRING_H_ */
