#include <jco/api/arraylist.h>
#include <arraylist-priv.h>

#include <string.h>

#include <jco/jco.h>

const struct Class *ArrayList = NULL;

static void
array_list_grow (struct ArrayList *self)
{
  void **arr = object_calloc (self->arr_size + self->growth_rate, sizeof (void *));

  memcpy (arr, ((struct ImmutableArrayList *)self)->arr, self->arr_size);
  object_free (((struct ImmutableArrayList *)self)->arr);
  ((struct ImmutableArrayList *)self)->arr = arr;

  self->arr_size += self->growth_rate;
}

void
initArrayList()
{
  initImmutableArrayList();
  if (!ArrayList)
    ArrayList = new (Class, "ArrayList", sizeof (struct ArrayList),
	construct, array_list_constructor,
	mutable_collection_add, array_list_add,
	list_add_at, array_list_add_at,
	list_set, array_list_set,
	list_sort, array_list_sort,
	0);

}

void
array_list_set_growth_rate (void *_self, unsigned int rate)
{
  preconditions_check_state (rate > 0);

  struct ArrayList *self = cast (_self, ArrayList);
  self->growth_rate = rate;
}

unsigned int
array_list_get_growth_rate (void *_self)
{
  struct ArrayList *self = cast (_self, ArrayList);
  return self->growth_rate;
}

void *
array_list_constructor (void *_self, va_list *app)
{
  struct ArrayList *self = super_construct (ArrayList, _self, app);
  self->growth_rate = ARRAY_LIST_DEFAULT_GROWTH_RATE;
  self->arr_size = ((struct ImmutableArrayList *)self)->size;

  return self;
}

bool
array_list_add (const void *_self, const void *o)
{
  struct ArrayList *self = cast (_self, ArrayList);
  struct ImmutableArrayList *super = (struct ImmutableArrayList *)self;

  if (!is_descendant (o, collection_content_type (self)))
    return false;

  if (collection_size (self) >= self->arr_size)
    array_list_grow (self);

  super->arr[super->size] = ref (o);
  super->size ++;

  return true;
}

bool
array_list_add_at (const void *_self, const void *o, unsigned int index)
{
  struct ArrayList *self = cast (_self, ArrayList);
  struct ImmutableArrayList *super = (struct ImmutableArrayList *)self;

  if (!is_descendant (o, collection_content_type (self)))
    return false;

  if (collection_size (self) >= self->arr_size)
    array_list_grow (self);

  for (int i = (self->arr_size - 1); i > index; i--)
    {
      if (i > 0)
	super->arr[i] = super->arr[i-1];
    }

  super->arr[index] = ref (o);
  return true;
}

void *
array_list_set (const void *_self, unsigned int index, const void *o)
{
  struct ArrayList *self = cast (_self, ArrayList);
    struct ImmutableArrayList *super = (struct ImmutableArrayList *)self;

    if (!is_descendant (o, collection_content_type (self)))
      {
	logger_logf (WARNING, "%O is the wrong type to set into this list.", class_of (o));
	return NULL;
      }

    void * ret = super->arr[index];
    super->arr[index] = ref (o);

    return ret;
}

void
array_list_sort (const void *_self, Comparator comparator)
{
  struct ImmutableArrayList *self = cast (_self, ImmutableArrayList);

  qsort (self->arr, self->size, size_of (self->content_type), comparator);
}
