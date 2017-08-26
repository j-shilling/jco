#ifndef INCLUDE_OBJECT_API_ARRAYLIST_H_
#define INCLUDE_OBJECT_API_ARRAYLIST_H_

#include <jco/base/class.h>
#include <jco/api/immutablearraylist.h>

extern const struct Class *ArrayList;

void initArrayList();

void array_list_set_growth_rate (void *_self, unsigned int rate);
unsigned int array_list_get_growth_rate (void *_self);


#endif /* INCLUDE_OBJECT_API_ARRAYLIST_H_ */
