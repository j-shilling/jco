#ifndef INCLUDE_OBJECT_API_LIST_H_
#define INCLUDE_OBJECT_API_LIST_H_

#include <stdbool.h>

#include <jco/api/collection.h>
#include <jco/api/comparator.h>
#include <jco/api/mutablecollection.h>

bool is_list (const void *_self);
bool is_mutable_list (const void *_self);
bool is_immutable_list (const void *_self);

void list_add_at (const void *_self, const void *o, unsigned int index);
bool list_add_all_at (const void *_self, const void *c, unsigned int index);
bool list_equals (const void *_self, const void *o);
void *list_get (const void *_self, unsigned int index);
int list_hash_code (const void *_self);
int list_index_of (const void *_self, const void *o);
int list_last_index_of (const void *_self, const void *o);
void *list_remove_at (const void *_self, unsigned int index);
void *list_set (const void *_self, unsigned int index, const void *o);
void list_sort (const void *_self, Comparator comparator);
void *list_sublist (const void *_self, unsigned int start, unsigned int end);


#endif /* INCLUDE_OBJECT_API_LIST_H_ */
