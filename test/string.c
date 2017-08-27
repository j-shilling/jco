#include <jco/jco.h>
#include <jco/api/string.h>

#include <check.h>
#include <string.h>

static const char *const test = "testtesttest";
static const char *const not = "laksjdf";

START_TEST (check_jco_new_string)
{
  void *str = jco_new (String, test);

  ck_assert_ptr_ne (str, 0);
  ck_assert_int_eq (0, strcmp (test, string_to_cstring(str)));
  ck_assert_int_eq (strlen (test), string_length (str));

  jco_unref (str);
}
END_TEST

START_TEST (check_equals)
{
  void *str = jco_new (String, test);

  ck_assert (equals (str, test));
  ck_assert (!equals (str, not));

  jco_unref (str);
}
END_TEST

START_TEST (check_char_at)
{
  void *str = jco_new (String, test);
  for (int i = 0; i < strlen (test); i ++)
    ck_assert (test[i] == string_char_at (str, i));
  jco_unref (str);
}
END_TEST

START_TEST (check_ends_with_cstring)
{
  void *str = jco_new (String, test);

  for (int i = (strlen (test) - 1); i >= 0; i--)
    ck_assert (string_ends_with (str, test + i));

  ck_assert (!string_ends_with (str, not));

  jco_unref (str);
}
END_TEST

START_TEST (check_ends_with_string)
{
  void *str = jco_new (String, test);

  for (int i = (strlen (test) - 1); i >= 0; i--)
    {
      void *other = jco_new (String, test + i);
      ck_assert (string_ends_with (str, other));
      jco_unref (other);
    }

  void *other = jco_new (String, not);
  ck_assert (!string_ends_with (str, other));
  jco_unref (other);

  jco_unref (str);
}
END_TEST

START_TEST (check_ends_with_char)
{
  void *str = jco_new (String, test);

  ck_assert (string_ends_with (str, test[strlen(test) - 1]));
  ck_assert ((test[strlen(test) - 1] == not[strlen(not) - 1])
	     == string_ends_with (str, not[strlen(not) - 1]));

  jco_unref (str);
}
END_TEST

Suite *
string_suite()
{
  initString();

  Suite *s = suite_create ("String");
  TCase *tc = tcase_create ("Core");

  tcase_add_test (tc, check_jco_new_string);
  tcase_add_test (tc, check_equals);
  tcase_add_test (tc, check_char_at);
  tcase_add_test (tc, check_ends_with_cstring);
  tcase_add_test (tc, check_ends_with_string);
  tcase_add_test (tc, check_ends_with_char);

  suite_add_tcase (s, tc);
  return s;
}
