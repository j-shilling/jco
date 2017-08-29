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
 * @file preconditions.h
 * @author Jake Shilling
 * @brief Macros to simplify error checking at th beginning of a
 *	  function. If the test fails, the programs exits with an
 *	  error message.
 */

#ifndef INCLUDE_OBJECT_PRECONDITIONS_H_
#define INCLUDE_OBJECT_PRECONDITIONS_H_

#include <jco/base/logger.h>
#include <stdlib.h>

/** Check that the given expression returns true */
#define jco_preconditions_check_state(expr)			\
  if (!(expr))						\
    {							\
      jco_log (SEVERE, 				\
	"%s:%d, %s returned false.",			\
	__FILE__, __LINE__, #expr);			\
      exit (EXIT_FAILURE);				\
    }
/** Check that the given expression returns true. If 
false, the given message is displayed. */
#define jco_preconditions_check_state_msg(expr, fmt, ...)	\
  if (!(expr))						\
    {							\
      jco_log (SEVERE, fmt, ##__VA_ARGS__);		\
      exit (EXIT_FAILURE);				\
    }

/** Check that the given expression returns true */
#define jco_preconditions_check_arg(expr)			\
 jco_preconditions_check_state (expr)
/** Check that the given expression returns true. If 
false, the given message is displayed. */
#define jco_preconditions_check_arg_msg(expr, fmt, ...)		\
  jco_preconditions_check_state_msg (expr, fmt, ##__VA_ARGS__)

/** Check that a given pointer is not null. */
#define jco_preconditions_check_not_null(ptr)		\
    if (!ptr)						\
      {							\
      jco_log (SEVERE, 				\
	"%s:%d, %s is null.",				\
	__FILE__, __LINE__, #ptr);			\
      exit (EXIT_FAILURE);				\
      }
/** Check that a given pointer is not null. If it is, log
the giveen message. */
#define jco_preconditions_check_not_null_msg(ptr, fmt, ...)	\
  if (!ptr)						\
    {							\
      jco_log (SEVERE, fmt, ##__VA_ARGS__);		\
      exit (EXIT_FAILURE);				\
    }

/** Check that the given index exists within a collection of
  the given size. */
#define jco_preconditions_check_index(index, size)		\
   if (index < 0 || index >= size)				\
     {								\
       jco_log (SEVERE,					\
	  "%s:%d, %d is not a valid index in a collection "	\
	  "or array of size %d",				\
	  __FILE__, __LINE__, index, size);			\
       exit (EXIT_FAILURE);					\
    }
/** Check that the given index exists within a collection of
  the given size. Log the given message if it is not. */
#define jco_preconditions_check_index_msg(index, fmt, ...)	\
   if (index < 0 || index >= size)				\
     {								\
       jco_log (SEVERE, fmt, ##__VA_ARGS__);		\
       exit (EXIT_FAILURE);					\
     }

/** Check that the given index is a valid position within a
  collection of a given size. */
#define jco_preconditions_check_position(index, size)		\
   if (index < 0 || index > size)				\
     {								\
       jco_log (SEVERE,					\
	  "%s:%d, %d is not a valid position in a collection "	\
	  "or array of size %d",				\
	  __FILE__, __LINE__, index, size);			\
       exit (EXIT_FAILURE);					\
     }
/** Check that the given index is a valid position within a
  collection of a given size. Log the message if not. */
#define jco_preconditions_check_position_msg(index, fmt, ...)	\
   if (index < 0 || index > size)				\
     {								\
       jco_log (SEVERE, fmt, ##__VA_ARGS__);		\
       exit (EXIT_FAILURE);					\
     }


#endif /* INCLUDE_OBJECT_PRECONDITIONS_H_ */
