#include <jco/api/immutablearraylist.h>
#include <immutablearraylist-priv.h>

#include <string.h>

#include <jco/jco.h>

const struct Class *ImmutableArrayList = NULL;
const struct Class *ArrayListIterator = NULL;

void initImmutableArrayList()
{
  if (!ArrayListIterator)
    ArrayListIterator = jco_new (Class, "ArrayListIterator", Object,
	sizeof (struct ArrayListIterator),
	construct, array_list_iterator_constructor,
	iterator_has_next, array_list_iterator_has_next,
	iterator_next, array_list_iterator_next,
	0);
  if (!ImmutableArrayList)
    ImmutableArrayList = jco_new (Class, "ImmutableArrayList", Object,
	sizeof (struct ImmutableArrayList),
	construct, immutable_array_list_constructor,
	destruct, immutable_array_list_destructor,
	to_string, collection_to_string,
	hash_code, list_hash_code,
	equals, list_equals,

	collection_content_type, array_list_content_type,
	collection_equals, list_equals,
	collection_iterator, array_list_iterator,
	collection_size, array_list_size,

	list_get, array_list_get,
	list_sublist, array_list_sublist,
	0);

}

void *
immutable_array_list_constructor (void *_self, va_list *app)
{
  struct ImmutableArrayList *self = super_construct (ImmutableArrayList, _self, app);
  self->content_type = va_arg (*app, const struct Class *);
  jco_preconditions_check_not_null (self->content_type);
  self->size = 0;
  self->arr = NULL;

  void **buf = jco_calloc (100, sizeof (void *));
  unsigned int buf_size = 100;

  void *item = 0;
  while ((item = va_arg (*app, void *)))
    {
      if (jco_is_descendant (item, collection_content_type (self)))
	{
	  if (self->size >= buf_size)
	    {
	      void **jco_newbuf = jco_calloc (100 + self->size, sizeof (void *));
	      memcpy (jco_newbuf, buf, buf_size);

	      jco_free (buf);
	      buf = jco_newbuf;
	      buf_size = self->size + 100;
	    }

	  buf[self->size] = jco_ref (item);
	  self->size ++;
	}
      else if (is_collection (item)
	  && jco_is_descendant (collection_content_type (item),
			    collection_content_type (self)))
	{
	  void *it = collection_iterator (item);

	  while (iterator_has_next (it))
	    {
	      if (self->size >= buf_size)
	      	    {
	      	      void **jco_newbuf = jco_calloc (100 + self->size, sizeof (void *));
	      	      memcpy (jco_newbuf, buf, buf_size);

	      	      jco_free (buf);
	      	      buf = jco_newbuf;
	      	      buf_size = self->size + 100;
	      	    }

	      	  buf[self->size] = iterator_next(it);
	      	  self->size ++;
	    }

	  jco_unref (it);
	}
      else
	{
	  jco_log (WARNING, "Cannot add items to this list which are not of type %O",
		       collection_content_type (self));
	  jco_free (buf);
	  return self;
	}
    }

  self->arr = jco_calloc (self->size, sizeof (void *));
  memcpy (self->arr, buf, self->size);
  jco_free (buf);

  return self;
}

void *
immutable_array_list_destructor (void *_self)
{
  struct ImmutableArrayList *self = jco_cast (ImmutableArrayList, _self);

  for (int i = 0; i < self->size; i ++)
    jco_unref (self->arr[i]);

  jco_free (self->arr);

  return super_destruct (ImmutableArrayList, self);
}

const struct Class *
array_list_content_type (const void *_self)
{
  struct ImmutableArrayList *self = jco_cast (ImmutableArrayList, _self);
  return self->content_type;
}

void *
array_list_iterator (const void *_self)
{
  return jco_new (ArrayListIterator, _self);
}

unsigned int
array_list_size (const void *_self)
{
  struct ImmutableArrayList *self = jco_cast (ImmutableArrayList, _self);
  return self->size;
}

void *
array_list_get (const void *_self, unsigned int index)
{
  struct ImmutableArrayList *self = jco_cast (ImmutableArrayList, _self);
  return jco_ref(self->arr[index]);
}

void *
array_list_sublist (const void *_self, unsigned int start, unsigned int end)
{
  struct ImmutableArrayList *self = jco_cast (_self, ImmutableArrayList);
  struct ImmutableArrayList *buf = jco_new (ImmutableArrayList, collection_content_type (self), 0);

  int size = (end - start) + 1;
  void **arr = jco_calloc (size, sizeof (void *));
  memcpy (arr, self->arr + start, size);

  buf->arr = arr;
  buf->size = size;

  void *ret = jco_new (jco_class_of (_self), collection_content_type (self), buf, 0);
  jco_unref (buf);

  return ret;
}

void *
array_list_iterator_constructor (void *_self, va_list *app)
{
  struct ArrayListIterator *self = super_construct (ArrayListIterator, _self, app);
  struct ImmutableArrayList *list =
      jco_cast (ImmutableArrayList, va_arg (*app, void *));

  self->arr = list->arr;
  self->size = list->size;
  self->cur = 0;

  return self;
}

bool
array_list_iterator_has_next (const void *_self)
{
  struct ArrayListIterator *self = jco_cast (_self, ArrayListIterator);
  return self->cur < self->size;
}

void *
array_list_iterator_next (const void *_self)
{
  struct ArrayListIterator *self = jco_cast (_self, ArrayListIterator);

  void *ret = jco_ref (self->arr[self->cur]);
  self->cur ++;
  return ret;
}
