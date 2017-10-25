#include <jco/api/hashset.h>

int check_collection (struct Class const *const class);
int check_mutable_collection (struct Class const *const class);

int
main (void)
{
  jco_init_hash_set();

  int number_failed = check_collection (ImmutableHashSet);
  number_failed += check_mutable_collection (HashSet);

  return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
