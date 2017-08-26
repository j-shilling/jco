#include <jco/api/immutablearraylist.h>
#include <immutablearraylist-priv.h>

#include <string.h>

#include <jco/jco.h>

const struct Class *ImmutableArrayList = NULL;
const struct Class *ArrayListIterator = NULL;

void initImmutableArrayList()
{
  if (!ArrayListIterator)
    ArrayListIterator = new (Class, "ArrayListIterator", Object,
	sizeof (struct ArrayListIterator),
	construct, array_list_iterator_constructor,
	iterator_has_next, array_list_iterator_has_next,
	iterator_next, array_list_iterator_next,
	0);
  if (!ImmutableArrayList)
    ImmutableArrayList = new (Class, "ImmutableArrayList", Object,
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
  preconditions_check_not_null (self->content_type);
  self->size = 0;
  self->arr = NULL;

  void **buf = object_calloc (100, sizeof (void *));
  unsigned int buf_size = 100;

  void *item = 0;
  while ((item = va_arg (*app, void *)))
    {
      if (is_descendant (item, collection_content_type (self)))
	{
	  if (self->size >= buf_size)
	    {
	      void **newbuf = object_calloc (100 + self->size, sizeof (void *));
	      memcpy (newbuf, buf, buf_size);

	      object_free (buf);
	      buf = newbuf;
	      buf_size = self->size + 100;
	    }

	  buf[self->size] = ref (item);
	  self->size ++;
	}
      else if (is_collection (item)
	  && is_descendant (collection_content_type (item),
			    collection_content_type (self)))
	{
	  void *it = collection_iterator (item);

	  while (iterator_has_next (it))
	    {
	      if (self->size >= buf_size)
	      	    {
	      	      void **newbuf = object_calloc (100 + self->size, sizeof (void *));
	      	      memcpy (newbuf, buf, buf_size);

	      	      object_free (buf);
	      	      buf = newbuf;
	      	      buf_size = self->size + 100;
	      	    }

	      	  buf[self->size] = iterator_next(it);
	      	  self->size ++;
	    }

	  unref (it);
	}
      else
	{
	  logger_logf (WARNING, "Cannot add items to this list which are not of type %O",
		       collection_content_type (self));
	  object_free (buf);
	  return self;
	}
    }

  self->arr = object_calloc (self->size, sizeof (void *));
  memcpy (self->arr, buf, self->size);
  object_free (buf);

  return self;
}

void *
immutable_array_list_destructor (void *_self)
{
  struct ImmutableArrayList *self = cast (ImmutableArrayList, _self);

  for (int i = 0; i < self->size; i ++)
    unref (self->arr[i]);

  object_free (self->arr);

  return super_destruct (ImmutableArrayList, self);
}

const struct Class *
array_list_content_type (const void *_self)
{
  struct ImmutableArrayList *self = cast (ImmutableArrayList, _self);
  return self->content_type;
}

void *
array_list_iterator (const void *_self)
{
  return new (ArrayListIterator, _self);
}

unsigned int
array_list_size (const void *_self)
{
  struct ImmutableArrayList *self = cast (ImmutableArrayList, _self);
  return self->size;
}

void *
array_list_get (const void *_self, unsigned int index)
{
  struct ImmutableArrayList *self = cast (ImmutableArrayList, _self);
  return ref(self->arr[index]);
}

void *
array_list_sublist (const void *_self, unsigned int start, unsigned int end)
{
  struct ImmutableArrayList *self = cast (_self, ImmutableArrayList);
  struct ImmutableArrayList *buf = new (ImmutableArrayList, collection_content_type (self), 0);

  int size = (end - start) + 1;
  void **arr = object_calloc (size, sizeof (void *));
  memcpy (arr, self->arr + start, size);

  buf->arr = arr;
  buf->size = size;

  void *ret = new (class_of (_self), collection_content_type (self), buf, 0);
  unref (buf);

  return ret;
}

void *
array_list_iterator_constructor (void *_self, va_list *app)
{
  struct ArrayListIterator *self = super_construct (ArrayListIterator, _self, app);
  struct ImmutableArrayList *list =
      cast (ImmutableArrayList, va_arg (*app, void *));

  self->arr = list->arr;
  self->size = list->size;
  self->cur = 0;

  return self;
}

bool
array_list_iterator_has_next (const void *_self)
{
  struct ArrayListIterator *self = cast (_self, ArrayListIterator);
  return self->cur < self->size;
}

void *
array_list_iterator_next (const void *_self)
{
  struct ArrayListIterator *self = cast (_self, ArrayListIterator);

  void *ret = ref (self->arr[self->cur]);
  self->cur ++;
  return ret;
}
