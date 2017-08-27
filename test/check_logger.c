#include <check.h>
#include <stdlib.h>
#include <stdbool.h>

#include <jco/jco.h>

START_TEST(hello_world)
{
  jco_log_level (FINEST);
  jco_log_file (FINEST, "test.log");
  jco_log_to_file (true);

  jco_log ("Hello, World!");
}
END_TEST

int
main (void)
{
  Suite *s = suite_create ("Logger");
  TCase *tc = tcase_create ("Core");

  tcase_add_test (tc, hello_world);

  suite_add_tcase (s, tc);

  SRunner *sr = srunner_create (s);

  srunner_set_fork_status (sr, CK_NOFORK);
  srunner_run_all (sr, CK_VERBOSE);
  int number_failed = srunner_ntests_failed (sr);
  srunner_free (sr);

  return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
