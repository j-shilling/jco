#include <jco/jco.h>
#include <jco/api/string.h>
#include <jco/util/asprintf.h>

#include <check.h>
#include <string.h>

static const char *const test = "testtesttest";
static const char *const not = "laksjdf";

START_TEST (check_jco_new_string)
{
  void *str = jco_new (String, test);

  ck_assert_ptr_ne (str, 0);
  ck_assert_int_eq (0, strcmp (test, jco_string_to_cstring(str)));
  ck_assert_int_eq (strlen (test), jco_string_length (str));

  jco_unref (str);
}
END_TEST

START_TEST (check_jco_new_jco_string_null_fmt)
{
  void *str = jco_new (String, 0);

  ck_assert_ptr_ne (str, 0);
  ck_assert_int_eq (0, strcmp ("", jco_string_to_cstring (str)));
  ck_assert_int_eq (0, jco_string_length (str));

  jco_unref (str);
}
END_TEST

START_TEST (check_jco_new_jco_string_complex)
{
  for (int i = 0; i < 100; i ++)
    {
      void *str = jco_new (String, "%d:%s", i, test);
      char *cmp = NULL;
      asprintf (&cmp, "%d:%s", i, test);

      ck_assert_ptr_ne (str, 0);
      ck_assert_int_eq (0, strcmp (cmp, jco_string_to_cstring (str)));

      jco_unref (str);
      free(cmp);
    }
}
END_TEST

START_TEST (check_equals)
{
  void *str = jco_new (String, test);

  ck_assert (jco_equals (str, test));
  ck_assert (!jco_equals (str, not));

  jco_unref (str);
}
END_TEST

START_TEST (check_char_at)
{
  void *str = jco_new (String, test);
  for (int i = 0; i < strlen (test); i ++)
    ck_assert (test[i] == jco_string_char_at (str, i));
  jco_unref (str);
}
END_TEST

START_TEST (check_ends_with_cstring)
{
  void *str = jco_new (String, test);

  for (int i = (strlen (test) - 1); i >= 0; i--)
    ck_assert (jco_string_ends_with (str, test + i));

  ck_assert (!jco_string_ends_with (str, not));

  jco_unref (str);
}
END_TEST

START_TEST (check_ends_with_string)
{
  void *str = jco_new (String, test);

  for (int i = (strlen (test) - 1); i >= 0; i--)
    {
      void *other = jco_new (String, test + i);
      ck_assert (jco_string_ends_with (str, other));
      jco_unref (other);
    }

  void *other = jco_new (String, not);
  ck_assert (!jco_string_ends_with (str, other));
  jco_unref (other);

  jco_unref (str);
}
END_TEST

START_TEST (check_ends_with_char)
{
  void *str = jco_new (String, test);

  ck_assert (jco_string_ends_with (str, test[strlen(test) - 1]));
  ck_assert ((test[strlen(test) - 1] == not[strlen(not) - 1])
	     == jco_string_ends_with (str, not[strlen(not) - 1]));

  jco_unref (str);
}
END_TEST

START_TEST (check_append_char)
{
  void *str = jco_new (String, "abc");
  jco_string_append (str, 'd');

  ck_assert_int_eq (0, strcmp ("abcd", jco_string_to_cstring (str)));

  jco_unref (str);
}
END_TEST

START_TEST (check_append_cstring)
{
  void *str = jco_new (String, "abc");
  jco_string_append (str, "def");

  ck_assert_int_eq (0, strcmp ("abcdef", jco_string_to_cstring (str)));

  jco_unref (str);
}
END_TEST

START_TEST (check_append_object)
{
  void *str = jco_new (String, "abc");
  void *str2 = jco_new (String, "def");
  jco_string_append (str, str2);

  ck_assert_int_eq (0, strcmp ("abcdef", jco_string_to_cstring (str)));

  jco_unref (str);
}
END_TEST


int
main (void)
{
  jco_init_string();
  Suite *s = suite_create ("String");
  TCase *tc = tcase_create ("Core");

  tcase_add_test (tc, check_jco_new_string);
  tcase_add_test (tc, check_jco_new_jco_string_null_fmt);
  tcase_add_test (tc, check_jco_new_jco_string_complex);
  tcase_add_test (tc, check_equals);
  tcase_add_test (tc, check_char_at);
  tcase_add_test (tc, check_ends_with_cstring);
  tcase_add_test (tc, check_ends_with_string);
  tcase_add_test (tc, check_ends_with_char);
  tcase_add_test (tc, check_append_char);
  tcase_add_test (tc, check_append_cstring);
  tcase_add_test (tc, check_append_object);

  suite_add_tcase (s, tc);

  SRunner *sr = srunner_create (s);

  srunner_set_fork_status (sr, CK_NOFORK);
  srunner_run_all (sr, CK_VERBOSE);
  int number_failed = srunner_ntests_failed (sr);
  srunner_free (sr);

  return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
