#ifndef INCLUDE_OBJECT_API_MUTABLECOLLECTION_H_
#define INCLUDE_OBJECT_API_MUTABLECOLLECTION_H_

#include <stdbool.h>

#include <jco/api/collection.h>
#include <jco/api/comparator.h>
#include <jco/api/iterator.h>

bool is_mutable_collection (const void *_self);

bool mutable_collection_add (const void *_self, const void *o);        // required
bool mutable_collection_add_all (const void *_self, const void *c);
void mutable_collection_clear (const void *_self);
bool mutable_collection_remove (const void *_self, const void *o);     // required
bool mutable_collection_remove_all (const void *_self, const void *c);
bool mutable_collection_retain_all (const void *_self, const void *c);

#endif /* INCLUDE_OBJECT_API_MUTABLECOLLECTION_H_ */
