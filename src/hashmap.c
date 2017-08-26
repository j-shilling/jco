#include <jco/api/hashmap.h>
#include <hashmap-priv.h>

#include <stdbool.h>
#include <stdarg.h>

#include <jco/jco.h>

/*
struct HashMap
{
  const struct Object _;

  const struct Class *key_type;
  const struct Class *item_type;

  const struct HashMapTable *table;
  unsigned int size;
}; */

struct HashMapTable *hash_map_table_create (unsigned int size);
struct HashMapTable *hash_map_table_destroy (struct HashMapTable *table);

/* From Object */
void *
hash_map_constructor (void *_self, va_list *app)
{
  struct HashMap *self = super_construct (HashMap, _self, app);

  self->key_type = va_arg (*app, const struct Class *);
  self->item_type = va_arg (*app, const struct Class *);
  self->table = hash_map_table_create (INIT_HASH_TABLE_SIZE);
  self->size = 0;

  return self;
}

void *
hash_map_destructor (void *_self)
{
  struct HashMap *self = cast (_self, HashMap);

  if (NULL != self->key_type)
    {
      void *es = hash_map_entry_set (self);
      void *it = collection_iterator (es);

      while (iterator_has_next (it))
	unref (iterator_next(it));

      unref (it);
      unref (es);
    }

  if (NULL != self->item_type)
    {
      void *is = hashmap_values(self);
      void *it = collection_iterator (is);

      while (iterator_has_next (it))
	unref (iterator_next (it));

      unref (it);
      unref (is);
    }

  hash_map_table_destroy (self->table);

  return super_destruct (HashMap, self);
}

bool
hash_map_equals (const void *_self, const void *o)
{

}
struct String *hash_map_to_string (const void *_self);
int hash_map_hash_code (const void *_self);

/* From Map */
void hash_map_clear (const void *_self);
bool hash_map_contains_key (const void *_self, const void *key);
bool hash_map_contains_value (const void *_self, const void *o);
void *hash_map_entry_set (const void *_self);
void *hash_map_get (const void *_self, const void *key);
bool hash_map_is_empty (const void *_self);
void *hash_map_key_set (const void *_self);
void *hash_map_put (const void *_self, const void *key, const void *value);
void *hash_map_put_all (const void *_self, const void *m);
unsigned int hash_map_size (const void *_self);
void *hash_map_values (const void *_self);
