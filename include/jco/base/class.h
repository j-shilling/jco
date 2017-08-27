#ifndef __CLASS_H__
#define __CLASS_H__

#include <stdbool.h>
#include <stdarg.h>
#include <stdlib.h>

#include <jco/base/logger.h>
#include <jco/base/object.h>

#define DEFINE_SELECTOR(name, default_method, _self, ...) 			\
      const struct Class *class = jco_class_of (_self); 				\
      if (!class) 								\
	{									\
	  jco_log (SEVERE, "This object does not have a class.");		\
	  exit (EXIT_FAILURE);							\
	}									\
      *(Method *)&default_method = 						\
	class_implements_method (class, (Selector) name) ?			\
	class_get_method (class, (Selector) name) : (Method) default_method;	\
      if (!default_method)							\
	{									\
	  jco_log (SEVERE, "%O does not implement " #name, class);		\
	  exit (EXIT_FAILURE);							\
	}									\
      return default_method (_self, ##__VA_ARGS__);


struct Class;
extern const struct Class *const Class;

typedef void (*Method)(void);
typedef void (*Selector)(void);

const size_t size_of (const void *_self);
const struct Class *super (const void *_self);

bool class_implements_method (const void *_self, Selector selector);
bool class_implements_all (const void *_self, ...);
Method class_get_method (const void *_self, Selector selector);

void *construct (void *_self, va_list *app);
void *super_construct (const struct Class *const class,
		       void *_self, va_list *app);
void *destruct (void *_self);
void *super_destruct (const struct Class *const class,
		      void *_self);
const struct String *to_string (const void *_self);
bool equals (const void *_self, const void *o);
int hash_code (const void *_self);

#endif /* __CLASS_H__ */
