#include <check.h>
#include <stdlib.h>

Suite *string_suite();

int
main (void)
{
  int number_failed;
  SRunner *sr;

  sr = srunner_create (string_suite());

  srunner_set_fork_status (sr, CK_NOFORK);
  srunner_run_all (sr, CK_VERBOSE);
  number_failed = srunner_ntests_failed (sr);
  srunner_free (sr);

  return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
