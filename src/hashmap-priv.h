#ifndef SRC_HASHMAP_PRIV_H_
#define SRC_HASHMAP_PRIV_H_

#include <object-priv.h>

#define INIT_HASH_TABLE_SIZE	3

struct HashMapBin
{
  const void *const key;
  const void *const item;

  const struct HashMapBin *next;
};

struct HashMapBin *hash_map_bin_create (void *key, void *item);
void hash_map_bin_destroy (struct HashMapBin *bin);

struct HashMapTable
{
  const struct HashMapBin **bins;
  unsigned int size;
};

struct HashMapTable *hash_map_table_create (unsigned int size);
struct HashMapTable *hash_map_table_destroy (struct HashMapTable *table);
void hash_map_table_cpy (struct HashMapTable *dest, struct HashMapTable *src);
unsigned int hash_map_get_index (const struct HashMapTable *table, const void *key);

struct HashMap
{
  const struct Object _;

  const struct Class *key_type;
  const struct Class *item_type;

  const struct HashMapTable *table;
  unsigned int size;
};

/* From Object */
void *hash_map_constructor (void *_self, va_list *app);
void *hash_map_destructor (void *_self);
bool hash_map_equals (const void *_self, const void *o);
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

#endif /* SRC_HASHMAP_PRIV_H_ */
