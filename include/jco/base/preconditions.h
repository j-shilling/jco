#ifndef INCLUDE_OBJECT_PRECONDITIONS_H_
#define INCLUDE_OBJECT_PRECONDITIONS_H_

#include <jco/base/logger.h>
#include <stdlib.h>

#define jco_preconditions_check_state(expr)			\
  if (!expr)						\
    {							\
      jco_log (SEVERE, 				\
	"%s:%d, %s returned false.",			\
	__FILE__, __LINE__, #expr);			\
      exit (EXIT_FAILURE);				\
    }
#define jco_preconditions_check_state_msg(expr, fmt, ...)	\
  if (!expr)						\
    {							\
      jco_log (SEVERE, fmt, ##__VA_ARGS__);		\
      exit (EXIT_FAILURE);				\
    }

#define jco_preconditions_check_arg(expr)			\
  jco_preconditions_check_state (expr)
#define jco_preconditions_check_arg_msg(expr, fmt, ...)		\
  jco_preconditions_check_state_msg (expr, fmt, ##__VA_ARGS__)

#define jco_preconditions_check_not_null(ptr)		\
    if (!ptr)						\
      {							\
      jco_log (SEVERE, 				\
	"%s:%d, %s is null.",				\
	__FILE__, __LINE__, #ptr);			\
      exit (EXIT_FAILURE);				\
      }
#define jco_preconditions_check_not_null_msg(ptr, fmt, ...)	\
  if (!ptr)						\
    {							\
      jco_log (SEVERE, fmt, ##__VA_ARGS__);		\
      exit (EXIT_FAILURE);				\
    }

#define jco_preconditions_check_index(index, size)		\
   if (index < 0 || index >= size)				\
     {								\
       jco_log (SEVERE,					\
	  "%s:%d, %d is not a valid index in a collection "	\
	  "or array of size %d",				\
	  __FILE__, __LINE__, index, size);			\
       exit (EXIT_FAILURE);					\
     }
#define jco_preconditions_check_index_msg(index, fmt, ...)	\
   if (index < 0 || index >= size)				\
     {								\
       jco_log (SEVERE, fmt, ##__VA_ARGS__);		\
       exit (EXIT_FAILURE);					\
     }

#define jco_preconditions_check_position(index, size)		\
   if (index < 0 || index > size)				\
     {								\
       jco_log (SEVERE,					\
	  "%s:%d, %d is not a valid position in a collection "	\
	  "or array of size %d",				\
	  __FILE__, __LINE__, index, size);			\
       exit (EXIT_FAILURE);					\
     }
#define jco_preconditions_check_position_msg(index, fmt, ...)	\
   if (index < 0 || index > size)				\
     {								\
       jco_log (SEVERE, fmt, ##__VA_ARGS__);		\
       exit (EXIT_FAILURE);					\
     }


#endif /* INCLUDE_OBJECT_PRECONDITIONS_H_ */
