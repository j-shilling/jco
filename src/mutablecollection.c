#include <jco/api/mutablecollection.h>
#include <jco/jco.h>

bool
mutable_collection_add_all_impl (const void *_self, const void *c)
{
  if (!is_collection (c))
    {
      logger_logf (WARNING, "%O is not a collection.", jco_class_of (c));
      return false;
    }

  bool ret = false;
  void *it = collection_iterator (c);
  while (iterator_has_next (it))
    ret = ret && mutable_collection_add (_self, jco_unref(iterator_next(it)));
  jco_unref (it);

  return ret;
}

bool
mutable_collection_remove_all_impl (const void *_self, const void *c)
{
  if (!is_collection (c))
      {
        logger_logf (WARNING, "%O is not a collection.", jco_class_of (c));
        return false;
      }

  bool ret = false;
  void *it = collection_iterator (c);
  while (iterator_has_next (it))
    ret = ret && mutable_collection_remove (_self, jco_unref(iterator_next(it)));
  jco_unref (it);

  return ret;
}

bool
mutable_collection_retain_all_impl (const void *_self, const void *c)
{
  if (!is_collection (c))
      {
        logger_logf (WARNING, "%O is not a collection.", jco_class_of (c));
        return false;
      }

  bool ret = false;
  void *it = collection_iterator (_self);
  while (iterator_has_next (it))
    {
      void * item = iterator_next (it);
      if (!collection_contains (c, item))
	ret = ret && mutable_collection_remove (_self, item);
      jco_unref (item);
    }
  jco_unref (it);
  return ret;
}

void
mutable_collection_clear_impl (const void *_self)
{
  void *it = collection_iterator (_self);
  while (iterator_has_next (it))
    mutable_collection_remove (_self, jco_unref (iterator_next(it)));
  jco_unref (it);
}

bool
is_mutable_collection (const void *_self)
{
  return is_collection (_self) && class_implements_all (_self,
				mutable_collection_add,
				mutable_collection_remove,
				0);
}

bool
mutable_collection_add (const void *_self, const void *o)
{
  bool (*dft) (const void *, const void *) = 0;
  DEFINE_SELECTOR (mutable_collection_add, dft, _self, o)
}

bool
mutable_collection_add_all (const void *_self, const void *c)
{
  bool (*dft) (const void *, const void *) = mutable_collection_add_all_impl;
  DEFINE_SELECTOR (mutable_collection_add_all, dft, _self, c)
}

void
mutable_collection_clear (const void *_self)
{
  void (*dft) (const void *) = mutable_collection_clear_impl;
  DEFINE_SELECTOR (mutable_collection_clear, dft, _self)
}

bool
mutable_collection_remove (const void *_self, const void *o)
{
  bool (*dft) (const void *, const void *) = 0;
  DEFINE_SELECTOR (mutable_collection_remove, dft, _self, o)
}

bool
mutable_collection_remove_all (const void *_self, const void *c)
{
  bool (*dft) (const void *, const void *) = mutable_collection_remove_all;
  DEFINE_SELECTOR (mutable_collection_remove_all, dft, _self, c)
}

bool
mutable_collection_retain_all (const void *_self, const void *c)
{
  bool (*dft) (const void *, const void *) = mutable_collection_retain_all_impl;
  DEFINE_SELECTOR (mutable_collection_retain_all, dft, _self, c)
}
