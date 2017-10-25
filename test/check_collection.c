#include <check.h>

#include <jco/jco.h>
#include <jco/api/collection.h>
#include <jco/api/string.h>

int check_iterator (struct Class const *const class);

static struct Class const *type = NULL;

START_TEST (check_new_collection_empty)
{
  void *c = jco_new (type, String, 0);

  ck_assert_ptr_ne (c, 0);
  ck_assert (collection_is_empty (c));
  ck_assert (collection_content_type (c) == String);
  ck_assert (is_collection (c));

  jco_unref (c);
}
END_TEST

START_TEST (check_new_collection_with_items)
{
  void *one = jco_new (String, "1");
  void *two = jco_new (String, "2");
  void *three = jco_new (String, "3");
  void *c = jco_new (type, String, one, two, three, 0);

  ck_assert_ptr_ne (c, 0);
  ck_assert (!collection_is_empty (c));
  ck_assert_int_eq (collection_size (c), 3);

  ck_assert (collection_contains (c, one));
  ck_assert (collection_contains (c, two));
  ck_assert (collection_contains (c, three));

  jco_unref (one);
  jco_unref (two);
  jco_unref (three);
  jco_unref (c);
}
END_TEST

START_TEST (check_new_collection_with_collection);
{
  void *one = jco_new (String, "1");
  void *two = jco_new (String, "2");
  void *three = jco_new (String, "3");
  void *src = jco_new (type, String, one, two, three, 0);
  void *c = jco_new (type, String, src, 0);

  ck_assert_ptr_ne (c, 0);
  ck_assert (!collection_is_empty (c));
  ck_assert_int_eq (collection_size (c), 3);

  ck_assert (collection_contains (c, one));
  ck_assert (collection_contains (c, two));
  ck_assert (collection_contains (c, three));

  jco_unref (one);
  jco_unref (two);
  jco_unref (three);
  jco_unref (src);
  jco_unref (c);
}
END_TEST

START_TEST (check_new_collection_with_items_and_collection);
{
  void *one = jco_new (String, "1");
  void *two = jco_new (String, "2");
  void *three = jco_new (String, "3");
  void *src = jco_new (type, String, two, three, 0);
  void *c = jco_new (type, String, one, src, 0);

  ck_assert_ptr_ne (c, 0);
  ck_assert (!collection_is_empty (c));
  ck_assert_int_eq (collection_size (c), 3);

  ck_assert (collection_contains (c, one));
  ck_assert (collection_contains (c, two));
  ck_assert (collection_contains (c, three));

  jco_unref (one);
  jco_unref (two);
  jco_unref (three);
  jco_unref (src);
  jco_unref (c);
}
END_TEST

START_TEST (check_collection_c_contains_all)
{
  void *one = jco_new (String, "1");
  void *two = jco_new (String, "2");
  void *three = jco_new (String, "3");
  void *test = jco_new (type, String, one, two, 0);
  void *c = jco_new (type, String, one, two, three, 0);

  ck_assert (collection_contains_all (c, test));
  ck_assert (!collection_contains_all (test, c));

  jco_unref (one);
  jco_unref (two);
  jco_unref (three);
  jco_unref (c);
  jco_unref (test);
}
END_TEST

int
check_collection (struct Class const *const class)
{
  jco_preconditions_check_not_null (class);
  type = class;

  jco_init_string();

  int number_failed = check_iterator (type);
  Suite *s = suite_create ("Collection");
  
  void *name = jco_to_string (class);
  TCase *tc = tcase_create (jco_string_to_cstring(name));
  jco_unref (name);

  tcase_add_test (tc, check_new_collection_empty);
  tcase_add_test (tc, check_new_collection_with_items);
  tcase_add_test (tc, check_new_collection_with_collection);
  tcase_add_test (tc, check_new_collection_with_items_and_collection);
  tcase_add_test (tc, check_collection_c_contains_all);

  suite_add_tcase (s, tc);

  SRunner *sr = srunner_create (s);

  srunner_set_fork_status (sr, CK_NOFORK);
  srunner_run_all (sr, CK_VERBOSE);
  number_failed += srunner_ntests_failed (sr);
  srunner_free (sr);

  return number_failed;
}
