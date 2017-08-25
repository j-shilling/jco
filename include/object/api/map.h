#ifndef INCLUDE_OBJECT_API_MAP_H_
#define INCLUDE_OBJECT_API_MAP_H_

#include <stdbool.h>

struct MapEntry
{
  const void *const key;
  const void *const value;
};

void map_clear (const void *_self);
bool map_contains_key (const void *_self, const void *key);
bool map_contains_value (const void *_self, const void *o);
void *map_entry_set (const void *_self);
void *map_get (const void *_self, const void *key);
bool map_is_empty (const void *_self);
void *map_key_set (const void *_self);
void *map_put (const void *_self, const void *key, const void *value);
void *map_put_all (const void *_self, const void *m);
unsigned int map_size (const void *_self);
void *map_values (const void *_self);

#endif /* INCLUDE_OBJECT_API_MAP_H_ */
