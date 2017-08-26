#ifndef __OBJECT_H__
#define __OBJECT_H__

#include <stdbool.h>
#include <stdarg.h>

#include <jco/base/class.h>

struct Object;
extern const struct Class *const Object;

void *new (const struct Class *class, ...);
void  delete (void *_self);

const struct Class *class_of (const void *_self);

bool is_object (const void *_self);
bool is_descendant (const void *_self, const struct Class *class);
bool is (const void *_self, const struct Class *class);

void *cast (const void *_self, const struct Class *class);

void *ref (const void *_self);
void *unref (const void *_self);

#endif /* __OBJECT_H__ */
