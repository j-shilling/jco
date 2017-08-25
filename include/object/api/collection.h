#ifndef INCLUDE_OBJECT_API_COLLECTION_H_
#define INCLUDE_OBJECT_API_COLLECTION_H_

#include <object/class.h>
#include <stdbool.h>

bool is_collection (const void *_self);

const struct Class *collection_content_type (const void *_self);
bool collection_contains (const void *_self, const void *o);
bool collection_contains_all (const void *_self, const void *c);
bool collection_equals (const void *_self, const void *c);
bool collection_is_empty (const void *_self);
void *collection_iterator (const void *_self);
unsigned int collection_size (const void *_self);
const struct String *collection_to_string (const void *_self);

#endif /* INCLUDE_OBJECT_API_COLLECTION_H_ */
