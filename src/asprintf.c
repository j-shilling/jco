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

#include <config.h>

#ifndef HAVE_APSRINTF

#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>
#include <stddef.h>

int
asprintf (char ** restrict __ptr,
          const char * restrict __fmt, ...)
{
  int size = 0;
  va_list args;

  va_start (args, __fmt);

  size = vsnprintf (NULL, size, __fmt, args);

  if (size < 0) { return -1; }

  *__ptr = (char *) malloc (size + 1);

  if (__ptr == NULL) { return -1; }

  size = vsprintf (*__ptr, __fmt, args);

  va_end (args);
  return size;
}

#endif /* HAVE_APSRINTF */
