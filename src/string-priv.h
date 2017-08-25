#ifndef __STRING_PRIV_H__
#define __STRING_PRIV_H__

#include <object-priv.h>

#include <stdbool.h>
#include <stdarg.h>

struct String
{
  const struct Object _;
  const char *text;
};

void *string_constructor (void *_self, va_list *app);
void *string_destructor (void *_self);
bool string_equals (const void *_self, const void *o);
struct String *string_to_string (const void *_self);
int string_hash_code (const void *_self);

#endif /* __STRING_PRIV_H__ */
