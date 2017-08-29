#ifndef SRC_ARRAYLIST_PRIV_H_
#define SRC_ARRAYLIST_PRIV_H_

#include <immutablearraylist-priv.h>

#define ARRAY_LIST_DEFAULT_GROWTH_RATE		10

struct ArrayList
{
  const struct ImmutableArrayList _;

  unsigned int arr_size;
  unsigned int growth_rate;
};

void *array_list_constructor (void *_self, va_list *app);

bool array_list_add_at (const void *_self, const void *o, unsigned int index);
bool array_list_add (const void *_self, const void *o);
void *array_list_set (const void *_self, unsigned int index, const void *o);
void array_list_sort (const void *_self, Comparator comparator);
bool array_list_remove (void const *_self, void const *o);
void * array_list_remove_at (void const *_self, unsigned int index);

#endif /* SRC_ARRAYLIST_PRIV_H_ */
