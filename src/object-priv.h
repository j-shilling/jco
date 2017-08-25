#ifndef __OBJECT_PRIV_H__
#define __OBJECT_PRIV_H__

#define MAGIC_NUMBER	0x0effaced

#include <stdbool.h>
#include <stdarg.h>

struct Object
{
  unsigned int magic_number;
  const struct Class *class;
};

void *object_constructor (void *_self, va_list *app);
void *object_destructor (void *_self);
bool object_equals (const void *_self, const void *o);
struct String *object_to_string (const void *_self);
int object_hash_code (const void *_self);

#endif /* __OBJECT_PRIV_H__ */
