#ifndef INCLUDE_OBJECT_MEMORY_H_
#define INCLUDE_OBJECT_MEMORY_H_

#include <stdlib.h>

void *object_malloc (const size_t size);
void  object_free (const void *ptr);
void *object_calloc (const size_t nmemb, const size_t size);
void *object_realloc (const void *ptr, const size_t size);

#endif /* INCLUDE_OBJECT_MEMORY_H_ */
