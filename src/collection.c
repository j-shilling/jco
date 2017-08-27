#include <jco/api/collection.h>

#include <jco/api/iterator.h>
#include <jco/api/string.h>

#include <jco/jco.h>


static unsigned int
collection_count (const void *_self, const void *o)
{
  unsigned int ret = 0;
  void *it = collection_iterator (_self);

  while (iterator_has_next (it))
    if (equals (o, jco_unref (iterator_next (it))))
      ret++;
  jco_unref (it);

  return ret;
}

static bool
collection_contains_impl (const void *_self, const void *o)
{
  if (!jco_is_descendant (o, collection_content_type (_self)))
    return false;

  void *it = collection_iterator (_self);
  while (iterator_has_next (it))
    if (equals (o, jco_unref (iterator_next(it))))
      return true;

  return false;
}

static bool
collection_contains_all_impl (const void *_self, const void *c)
{
  if (!is_collection (c))
    {
      logger_logf (WARNING, "%O is not a collection.",
		   jco_class_of (c));
      return false;
    }

  if (collection_is_empty (c))
    {
      logger_log (WARNING, "Checking whether this contains all of an empty collection.");
      return true;
    }

  void *it = collection_iterator (c);
  while (iterator_has_next (it))
    {
      void *item = iterator_next (it);
      if (!collection_contains (_self, item))
	{
	  jco_unref (item);
	  jco_unref (it);
	  return false;
	}
      jco_unref (item);
    }
  jco_unref (it);
  return true;
}

static bool
collection_is_empty_impl (const void *_self)
{
  return collection_size (_self) == 0;
}

static bool
collection_equals_impl (const void *_self, const void *c)
{
  if (!is_collection(c))
    return false;

  if (collection_size (_self) != collection_size (c))
    return false;

  void *it = collection_iterator (_self);
  while (iterator_has_next(it))
    {
      void *item = iterator_next(it);
      if (collection_count (_self, item) != collection_count (_self, c))
	{
	  jco_unref (it);
	  jco_unref(item);
	  return false;
	}
      jco_unref (item);
    }

  jco_unref (it);
  return true;
}

bool
is_collection (const void *_self)
{
  return class_implements_all (_self,
			       collection_content_type,
			       collection_iterator,
			       collection_size,
			       0);
}

bool
collection_equals (const void *_self, const void *c)
{
  bool (*dft) (const void *, const void *)
      = collection_equals_impl;
  DEFINE_SELECTOR (collection_equals, dft, _self, c);
}

const struct String *
collection_to_string (const void *_self)
{
  void *it = collection_iterator (_self);
  const struct String *ret = jco_new (String, "{");

  while (iterator_has_next (it))
    {
      string_append (ret, jco_unref (iterator_next (it)));
      if (iterator_has_next (it))
	string_append (ret, ", ");
    }
  string_append (ret, "}");
  jco_unref (it);
  return ret;
}

const struct Class *
collection_content_type (const void *_self)
{
  jco_preconditions_check_state (is_collection (_self));
  struct Class *(*dft) (const void *) = 0;
  DEFINE_SELECTOR (collection_content_type, dft, _self)
}

bool
collection_contains (const void *_self, const void *o)
{
  bool (*dft) (const void *, const void *) = collection_contains_impl;
  DEFINE_SELECTOR (collection_contains, dft, _self, o)
}

bool
collection_contains_all (const void *_self, const void *c)
{
  bool (*dft) (const void *, const void *) = collection_contains_all_impl;
  DEFINE_SELECTOR (collection_contains_all, dft, _self, c)
}

bool
collection_is_empty (const void *_self)
{
  bool (*dft) (const void *) = collection_is_empty_impl;
  DEFINE_SELECTOR (collection_is_empty, dft, _self)
}

void *
collection_iterator (const void *_self)
{
  void *(*dft) (const void *) = 0;
  DEFINE_SELECTOR (collection_iterator, dft, _self)
}

unsigned int
collection_size (const void *_self)
{
  unsigned int (*dft) (const void *) = 0;
  DEFINE_SELECTOR (collection_size, dft, _self)
}
