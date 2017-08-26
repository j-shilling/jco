#ifndef INCLUDE_OBJECT_API_ITERATOR_H_
#define INCLUDE_OBJECT_API_ITERATOR_H_

#include <stdbool.h>

bool is_iterator (const void *_self);
bool iterator_has_next (const void *_self);
void *iterator_next (const void *_self);

#endif /* INCLUDE_OBJECT_API_ITERATOR_H_ */
