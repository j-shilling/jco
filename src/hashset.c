#include <jco/jco.h>
#include <jco/api/arraylist.h>
#include <jco/api/hashset.h>

#include <hashset-priv.h>
#include <object-priv.h>

#include <stdarg.h>
#include <stddef.h>
#include <math.h>

struct Class const *HashSet = NULL;
struct Class const *ImmutableHashSet = NULL;
struct Class const *HashSetIterator = NULL;

struct HashSetIterator
{
  struct Object const _;
  void **arr;
  unsigned int size;
  unsigned int cur;
  void *it;

  void *next;
};

void *
hash_set_iterator_constructor (void *_self, va_list *ap)
{
  struct HashSetIterator *self = jco_super_construct (HashSetIterator, _self, ap);
  struct ImmutableHashSet *set = jco_cast (va_arg (*ap, void *), ImmutableHashSet);

  self->arr = set->arr;
  self->size = set->size;

  self->cur = 0;
  self->it = 0;
  self->next = 0;

  if (self->arr)
    {
      while (!self->arr[self->cur] && self->cur < self->size)
	self->cur++;

      if (self->arr[self->cur])
	self->it = collection_iterator (self->arr[self->cur]);

      if (self->it && iterator_has_next (self->it))
	self->next = iterator_next (self->it);
    }

  return self;
}

void *
hash_set_iterator_next (void const *const _self)
{
  struct HashSetIterator *self = jco_cast (_self, HashSetIterator);

  void *ret = self->next;
  self->next = 0;

  while (!self->next && (self->cur < self->size))
    {
      if (self->it)
	{
	  if (iterator_has_next (self->it))
	    {
	      self->next = iterator_next (self->it);
	    }
	  else
	    {
	      jco_unref (self->it);
	      self->it = 0;
	      self->cur++;
	    }
	}
      else
	{
	  if (self->arr[self->cur])
	    self->it = collection_iterator (self->arr[self->cur]);
	  else
	    self->cur++;
	}
    }

  return ret;
}

bool
hash_set_iterator_has_next (void const *const _self)
{
  struct HashSetIterator *self = jco_cast (_self, HashSetIterator);
  return self->next != NULL;
}

static void
jco_hash_set_cpy (void **const dest, size_t const dest_size,
	          void **const src, size_t const src_size)
{
  for (int i = 0; i < src_size; i++)
    {
      if (src[i])
	{
	  void *it = collection_iterator (src[i]);
	  while (iterator_has_next (it))
	    {
	      void *item = iterator_next (it);
	      unsigned int index = jco_hash_code (item) % dest_size;
	      if (!dest[index])
		{
		  dest[index] = jco_new (is_mutable_collection (src[i]) ?
					 ArrayList : ImmutableArrayList,
					 jco_class_of (item), item, 0);
		}
	      else
		{
		  if (is_mutable_collection (dest[index]))
		    {
		      mutable_collection_add (dest[index], item);
		    }
		  else
		    {
		      void *temp = dest[index];
		      dest[index] = jco_new (ImmutableArrayList,
					     jco_class_of (item), temp, item, 0);
		      jco_unref (temp);
		    }
		}
	      jco_unref (item);
	    }
	  jco_unref (it);
	}
    }
}

static bool
jco_is_prime (unsigned int const x)
{
  double lmt = sqrt (x);
  for (int i = 2; i <= lmt; i++)
    if ((x % i) == 0)
      return false;

  return true;
}

void
jco_init_hash_set()
{
  initArrayList();
  if (!ImmutableHashSet)
    ImmutableHashSet = jco_new (Class, "ImmutableHashSet", Object,
				sizeof (struct ImmutableHashSet),
				jco_construct, jco_immutable_hash_set_constructor,
				jco_destruct, jco_immutable_hash_set_destructor,
				jco_to_string, collection_to_string,
				jco_equals, collection_equals,

				collection_content_type, jco_hash_set_content_type,
				collection_iterator, jco_hash_set_iterator,
				collection_size, jco_hash_set_size,
				collection_contains, jco_hash_set_contains,
				0);
  if (!HashSet)
    HashSet = jco_new (Class, "HashSet", ImmutableHashSet,
		       sizeof (struct HashSet),
		       mutable_collection_add, jco_hash_set_add,
		       mutable_collection_remove, jco_hash_set_remove,
		       0);

  if (!HashSetIterator)
    HashSetIterator = jco_new (Class, "HashSetIterator", Object,
			       sizeof (struct HashSetIterator),
			       jco_construct, hash_set_iterator_constructor,
			       iterator_next, hash_set_iterator_next,
			       iterator_has_next, hash_set_iterator_has_next,
			       0);
}

void *
jco_immutable_hash_set_constructor (void *_self, va_list *ap)
{
  struct ImmutableHashSet *self = jco_super_construct (ImmutableHashSet, _self, ap);

  self->type = va_arg (*ap, struct Class *);
  self->arr = 0;
  self->nmemb = 0;
  self->size = 0;

  void *item = 0;
  while  ((item = va_arg (*ap, void *)))
    {
      if (is_collection (item))
	{
	  void *it = collection_iterator (item);
	  void *x = 0;
	  while (iterator_has_next (it))
	    {
	      x = iterator_next (it);
	      jco_hash_set_add (self, x);
	      jco_unref (x);
	    }
	  jco_unref (it);
	}
      else if (jco_is_descendant (item, self->type))
	{
	  jco_hash_set_add (self, item);
	}
      else
	{
	  jco_log (WARNING, "Cannot add a %O to a set of type %O",
				jco_class_of (item),
				self->type);
	}	  
    }

  return self;
}

void *
jco_immutable_hash_set_destructor (void *_self)
{
  struct ImmutableHashSet *self = jco_cast (_self, ImmutableHashSet);
  if (self->arr)
    {
      for (int i = 0; i < self->size; i ++)
	jco_unref (self->arr[i]);

      jco_free (self->arr);
    }

  return jco_super_destruct (ImmutableHashSet, _self);
}

struct Class *
jco_hash_set_content_type (void const *const _self)
{
  struct ImmutableHashSet *self = jco_cast (_self, ImmutableHashSet);
  return (struct Class *)self->type;
}

void *
jco_hash_set_iterator (void const *const _self)
{
  return jco_new (HashSetIterator, _self);
}

unsigned int
jco_hash_set_size (void const *const _self)
{
  struct ImmutableHashSet *self = jco_cast (_self, ImmutableHashSet);
  return self->nmemb;
}

bool
jco_hash_set_contains (void const *const _self, void const *const o)
{
  struct ImmutableHashSet *self = jco_cast (_self, ImmutableHashSet);
  if (!jco_is_descendant (o, self->type))
    return false;

  unsigned int index = jco_hash_code (o) % self->size;
  if (self->arr[index])
    return collection_contains (self->arr[index], o);
  else
    return false;
}

bool
jco_hash_set_add (void const *const _self, void const *const o)
{
  struct ImmutableHashSet *self = jco_cast (_self, ImmutableHashSet);
  if (!jco_is_descendant (o, self->type))
    return false;

  if (self->size == 0 || ((double)(self->nmemb+1) / (double)self->size) >= MAX_RATIO)
    {
      unsigned i = (self->size%2) == 0 ? self->size + 1 : self->size + 2;
      if (i < 3) i = 3;

      while (!jco_is_prime (i))
	i += 2;

      void **arr = jco_calloc (i, sizeof (void *));
      jco_hash_set_cpy (arr, i, self->arr, self->size);
      for (int j = 0; j < self->size; j++)
	if (self->arr[j])
	  jco_unref (self->arr[j]);
      jco_free (self->arr);

      self->size = i;
      self->arr = arr;
    }

  unsigned int i = jco_hash_code (o) % self->size;
  if (!self->arr[i])
    {
      self->arr[i] = jco_new (is_mutable_collection (self) ? 
				ArrayList : ImmutableArrayList,
			      self->type,
			      o, 0);
    }
  else
    {
      if (is_mutable_collection (self->arr[i]))
	mutable_collection_add (self->arr[i], o);
      else
	{
	  void *list = self->arr[i];
	  self->arr[i] = jco_new (ImmutableArrayList, self->type, list, o, 0);
	  jco_unref (list);
	}
    }
  self->nmemb++;
  return true;
}

bool
jco_hash_set_remove (void const *const _self, void const *const o)
{
  struct ImmutableHashSet *self = jco_cast (_self, ImmutableHashSet);
  jco_preconditions_check_arg (is_mutable_collection (self));

  if (!jco_is_descendant (o, self->type))
    return false;

  unsigned int i = jco_hash_code (o) % self->size;
  void *list = self->arr[i];

  if (!list)
    return false;

  if (!mutable_collection_remove (list, o))
    return false;

  self->nmemb --;

  if ((self->nmemb > 0) && ((double) (self->nmemb) / (double) (self->size) <= MIN_RATIO))
    {
      unsigned int new = (self->size % 2) == 0 ?
			    self->size - 1 : self->size - 2;
      while (!jco_is_prime (new) && new > 0)
	new -= 2;	

      void **arr = jco_calloc (new, sizeof (void *));
      jco_hash_set_cpy (arr, new, self->arr, self->size);

      for (int j = 0; j < self->size; j++)
	if (self->arr[j])
	  jco_unref (self->arr[j]);
      jco_free (self->arr);

      self->size = new;
      self->arr = arr;
    }

  return true;
}
