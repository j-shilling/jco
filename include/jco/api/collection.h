#ifndef INCLUDE_OBJECT_API_COLLECTION_H_
#define INCLUDE_OBJECT_API_COLLECTION_H_

#include <stdbool.h>
#include <jco/base/class.h>

bool is_collection (const void *_self);

const struct Class *collection_content_type (const void *_self); // required
bool collection_contains (const void *_self, const void *o);
bool collection_contains_all (const void *_self, const void *c);
bool collection_equals (const void *_self, const void *c);
bool collection_is_empty (const void *_self);
void *collection_iterator (const void *_self);			// required
unsigned int collection_size (const void *_self);		// required
const struct String *collection_to_string (const void *_self);

#endif /* INCLUDE_OBJECT_API_COLLECTION_H_ */
