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
 * @file hash.h
 * @author Jake Shilling
 * @brief Functions for generating hash codes
 */

#ifndef __HASH_H__
#define __HAHS_H__

#define jco_hash(x)                    \
  _Generic ((x),                       \
	    char: jco_hash_int,        \
   unsigned char: jco_hash_int,        \
     signed char: jco_hash_int         \
				       \
	     int: jco_hash_int         \
    unsigned int: jco_hash_int         \
			               \
           short: jco_hash_int         \
  unsigned short: jco_hash_int         \
	                               \
            long: jco_hash_long_long   \
   unsigned long: jco_hash_long_long   \
	                               \
           float: jco_hash_long_double \
          double: jco_hash_long_double \
     long double: jco_hash_long_double \
	                               \
	  char *: jco_hash_string      \
         default: jco_hash_ptr) (x)


int jco_hash_int (int const val);
int jco_hash_long_long (long long const val);
int jco_hash_long_double (long double const val);

int jco_hash_string (char const *const val);
int jco_hash_object (void const *const val);
int jco_hash_ptr (void const *const val);

#endif /* __HASH_H__ */
