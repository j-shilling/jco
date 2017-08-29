#include <jco/api/iterator.h>
#include <jco/jco.h>

bool
is_iterator (const void *_self)
{
  return jco_class_implements_all (_self, iterator_has_next, iterator_next, 0);
}

bool
iterator_has_next (const void *_self)
{
  bool (*default_impl) (const void *) = 0;
  DEFINE_SELECTOR (iterator_has_next, default_impl, _self)
}

void *
iterator_next (const void *_self)
{
  void *(*default_impl) (const void *) = 0;
  DEFINE_SELECTOR (iterator_next, default_impl, _self)
}
