#include <jco/api/arraylist.h>

int check_collection (struct Class const *const class);
int check_mutable_collection (struct Class const *const class);

int
main (void)
{
  initArrayList();

  int number_failed = check_collection (ImmutableArrayList);
  number_failed += check_mutable_collection (ArrayList);

  return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
