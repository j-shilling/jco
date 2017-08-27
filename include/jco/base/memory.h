#ifndef INCLUDE_OBJECT_MEMORY_H_
#define INCLUDE_OBJECT_MEMORY_H_

#include <stdlib.h>

void *jco_malloc (const size_t size);
void  jco_free (const void *ptr);
void *jco_calloc (const size_t nmemb, const size_t size);
void *jco_realloc (const void *ptr, const size_t size);

#endif /* INCLUDE_OBJECT_MEMORY_H_ */
