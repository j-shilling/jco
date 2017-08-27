#include <jco/api/list.h>
#include <jco/api/iterator.h>

#include <jco/jco.h>

static bool
list_add_all_at_impl (const void *_self, const void *c, unsigned int index)
{
  if (collection_is_empty (c))
    return false;

  unsigned int i = index;
  void *it = collection_iterator (c);

  while (iterator_has_next)
    {
      void *item = iterator_next (it);
      list_add_at (_self, item, i);
      i++;
      jco_unref (item);
    }
  jco_unref (it);

  return true;
}

static int
list_index_of_impl (const void *_self, const void *o)
{
  int size = collection_size (_self);
  for (int i = 0; i < size; i++)
    if (equals (o, jco_unref (list_get (_self, i))))
      return i;

  return -1;
}

static int
list_last_index_of_impl (const void *_self, const void *o)
{
  int size = collection_size (_self);
    for (int i = size; i >= 0; i++)
      if (equals (o, jco_unref (list_get (_self, i))))
        return i;

    return -1;
}

static void *
list_remove_at_impl (const void *_self, unsigned int index)
{
  void *item = list_get (_self, index);

  mutable_collection_remove (_self, item);

  return item;
}

static void *
list_set_impl (const void *_self, unsigned int index, const void *o)
{
  void *item = list_get (_self, index);

  if (item)
    {
      mutable_collection_remove (_self, item);
    }
  list_add_at (_self, o, index);

  return item;
}

static void
list_sort_impl (const void *_self, Comparator comparator)
{
  unsigned int size = collection_size (_self);
  for (int n = 0; n < size; n ++)
    {
      bool changed = false;

      for (int i = 0; i < (size - 1); i ++)
	{
	  void *x = list_get (_self, i);
	  void *y = list_get (_self, i+1);

	  if (comparator (x, y) > 0)
	    {
	      list_set (_self, i, y);
	      list_set (_self, i+1, x);
	    }

	  jco_unref (x);
	  jco_unref (y);
	}

      if (!changed)
	return;
    }
}

static void *
list_sublist_impl (const void *_self, unsigned int start, unsigned int end)
{
  jco_preconditions_check_state (is_mutable_list (_self));

  void *ret = jco_new (jco_class_of (_self));
  for (int i = start; i <= end; i++)
    mutable_collection_add (ret, jco_unref (list_get(_self, i)));

  return ret;
}

bool
is_list (const void *_self)
{
  return _self
      && is_collection (_self)
      && class_implements_method (_self, (Selector) list_get);
}

bool
is_mutable_list (const void *_self)
{
  return is_list (_self)
      && is_mutable_collection (_self)
      && class_implements_all (_self, list_set, list_add_at);
}

bool
is_immutable_list (const void *_self)
{
  return is_list (_self)
        && !is_mutable_collection (_self)
        && !class_implements_method (_self, (Selector) list_set);
}

void
list_add_at (const void *_self, const void *o, unsigned int index)
{
  jco_preconditions_check_state (is_mutable_list (_self));
  jco_preconditions_check_position (index, collection_size (_self));

  void (*dft) (const void *, const void *, unsigned int) = 0;
  DEFINE_SELECTOR (list_add_at, dft, _self, o, index);
}

bool
list_add_all_at (const void *_self, const void *c, unsigned int index)
{
  jco_preconditions_check_state (is_mutable_list (_self));
  jco_preconditions_check_state (is_collection (c));
  jco_preconditions_check_position (index, collection_size (_self));

  bool (*dft) (const void *, const void *, unsigned int) =
      list_add_all_at_impl;
  DEFINE_SELECTOR (list_add_at, dft, _self, c, index);
}

void *
list_get (const void *_self, unsigned int index)
{
  jco_preconditions_check_state (is_mutable_list (_self));
  jco_preconditions_check_index (index, collection_size (_self));

  void *(*dft) (const void *, unsigned int) = 0;
  DEFINE_SELECTOR (list_get, dft, _self, index);
}

int
list_index_of (const void *_self, const void *o)
{
  jco_preconditions_check_state (is_list (_self));

  int (*dft) (const void *, const void *) =
      list_index_of_impl;
  DEFINE_SELECTOR (list_index_of, dft, _self, o);
}

int
list_last_index_of (const void *_self, const void *o)
{
  jco_preconditions_check_state (is_list (_self));

  int (*dft) (const void *, const void *) =
      list_last_index_of_impl;
  DEFINE_SELECTOR (list_last_index_of, dft, _self, o);
}

void *
list_remove_at (const void *_self, unsigned int index)
{
  jco_preconditions_check_state (is_mutable_list (_self));
  jco_preconditions_check_index (index, collection_size (_self));

  void *(*dft) (const void *, unsigned int) =
      list_remove_at_impl;
  DEFINE_SELECTOR(list_remove_at, dft, _self, index);
}

void *
list_set (const void *_self, unsigned int index, const void *o)
{
  jco_preconditions_check_state (is_mutable_list (_self));
  jco_preconditions_check_position (index, collection_size (_self));

  void * (*dft) (const void *, unsigned int, const void *) =
      list_set_impl;
  DEFINE_SELECTOR (list_set, dft, _self, index, o);
}

void
list_sort (const void *_self, Comparator comparator)
{
  jco_preconditions_check_state (is_mutable_list (_self));
  jco_preconditions_check_not_null (comparator);

  void (*dft) (const void *, Comparator) =
      list_sort_impl;
  DEFINE_SELECTOR (list_sort, dft, _self, comparator);
}

void *
list_sublist (const void *_self, unsigned int start, unsigned int end)
{
  jco_preconditions_check_state (is_list (_self));
  jco_preconditions_check_index (start, collection_size (_self));
  jco_preconditions_check_index (end, collection_size (_self));
  jco_preconditions_check_state (start <= end);

  void *(*dft)(const void *, unsigned int, unsigned int) =
      list_sublist_impl;
  DEFINE_SELECTOR (list_sublist, dft, _self, start, end);
}

bool
list_equals (const void *_self, const void *o)
{
  if (!is_list (o))
    return false;

  unsigned int size = collection_size (_self);
  if (collection_size (o) != size)
    return false;

  for (int i = 0; i < size; i++)
    if (!equals(jco_unref(list_get(_self, i)), jco_unref(list_get (o, i))))
	return false;

  return true;
}

int
list_hash_code (const void *_self)
{
  int ret = 1;

  void *it = collection_iterator (_self);
  while (iterator_has_next (it))
    {
      void *item = iterator_next(it);
      ret = (31*ret) + (item == NULL ? 0 : hash_code (item));
      jco_unref (item);
    }
  jco_unref (it);

  return ret;
}
