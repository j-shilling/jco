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
 * @file parsefmt.h
 * @author Jake Shilling
 * @breif Define functions to parse printf-style format strings
 */

#include <stdarg.h>
#include <stddef.h>

/**
 * @brief Parse a printf format string, with the added functuality of
 *	  using %O to convert a jco object to a string.
 *
 * @param buf	  A pointer to where the resulting string should go. New
 *		  memory will be allocated with jco_malloc and must be
 *		  freed.
 * @param fmt	  The format string
 * @param ap	  A list of arguments to be added to the string.
 * @returns	  The size of the allocated memory
 */
size_t parsefmt (char **const restrict buf,
	      char const *const restrict fmt,
	      va_list ap);
