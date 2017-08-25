#ifndef SRC_IMMUTABLEARRAYLIST_PRIV_H_
#define SRC_IMMUTABLEARRAYLIST_PRIV_H_

#include <object-priv.h>

#include <object/class.h>

struct ImmutableArrayList
{
  const struct Object _;

  const struct Class *content_type;
  unsigned int size;
  void **arr;
};

void *immutable_array_list_constructor (void *_self, va_list *app);
void *immutable_array_list_destructor (void *_self);

const struct Class *array_list_content_type (const void *_self);
void *array_list_iterator (const void *_self);
unsigned int array_list_size (const void *_self);
void *array_list_get (const void *_self, unsigned int index);
void *array_list_sublist (const void *_self, unsigned int start, unsigned int end);

extern const struct Class *ArrayListIterator;

struct ArrayListIterator
{
  const struct Object _;

  void ** arr;
  unsigned int size;
  unsigned int cur;
};

void *array_list_iterator_constructor (void *_self, va_list *app);

bool array_list_iterator_has_next (const void *_self);
void *array_list_iterator_next (const void *_self);

#endif /* SRC_IMMUTABLEARRAYLIST_PRIV_H_ */
