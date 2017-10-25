#include <check.h>

#include <jco/jco.h>
#include <jco/api/mutablecollection.h>
#include <jco/api/string.h>

int check_collection (struct Class const *const class);
static struct Class const *type = NULL;

START_TEST (check_new_mutable_collection)
{
  void *c = jco_new (type, String, 0);
  ck_assert (is_mutable_collection (c));
  jco_unref (c);
}
END_TEST

START_TEST (check_mutable_collection_add)
{
  void *one = jco_new (String, "1");
  void *two = jco_new (String, "2");
  void *three = jco_new (String, "3");
  void *four = jco_new (String, "4");
  void *c = jco_new (type, String, one, two, three, 0);

  ck_assert_int_eq (collection_size (c), 3);
  ck_assert (collection_contains (c, one));
  ck_assert (collection_contains (c, two));
  ck_assert (collection_contains (c, three));
  ck_assert (!collection_contains (c, four));

  ck_assert (mutable_collection_add (c, four));
  ck_assert_int_eq (collection_size (c), 4);
  ck_assert (collection_contains (c, one));
  ck_assert (collection_contains (c, two));
  ck_assert (collection_contains (c, three));
  ck_assert (collection_contains (c, four));

  int i = 1;
  ck_assert (!mutable_collection_add (c, &i));

  jco_unref (one);
  jco_unref (two);
  jco_unref (three);
  jco_unref (four);
  jco_unref (c);
}
END_TEST

START_TEST (check_mutable_collection_add_all)
{
  void *one = jco_new (String, "1");
  void *two = jco_new (String, "2");
  void *three = jco_new (String, "3");
  void *four = jco_new (String, "4");
  void *c = jco_new (type, String, one, two, 0);
  void *cc = jco_new (type, String, three, four, 0);

  ck_assert_int_eq (collection_size (c), 2);
  ck_assert (collection_contains (c, one));
  ck_assert (collection_contains (c, two));
  ck_assert (!collection_contains (c, three));
  ck_assert (!collection_contains (c, four));

  ck_assert (mutable_collection_add_all (c, cc));
  ck_assert_int_eq (collection_size (c), 4);
  ck_assert (collection_contains (c, one));
  ck_assert (collection_contains (c, two));
  ck_assert (collection_contains (c, three));
  ck_assert (collection_contains (c, four));

  jco_unref (one);
  jco_unref (two);
  jco_unref (three);
  jco_unref (four);
  jco_unref (c);
  jco_unref (cc);
}
END_TEST

START_TEST (check_mutable_collection_clear)
{
  void *one = jco_new (String, "1");
  void *two = jco_new (String, "2");
  void *three = jco_new (String, "3");
  void *c = jco_new (type, String, one, two, three, 0);

  ck_assert_int_eq (collection_size (c), 3);
  ck_assert (collection_contains (c, one));
  ck_assert (collection_contains (c, two));
  ck_assert (collection_contains (c, three));

  mutable_collection_clear (c);

  ck_assert (collection_is_empty (c));

  jco_unref (one);
  jco_unref (two);
  jco_unref (three);
  jco_unref (c);
}
END_TEST
 
START_TEST (check_mutable_collection_remove)
{
  void *one = jco_new (String, "1");
  void *two = jco_new (String, "2");
  void *three = jco_new (String, "3");
  void *four = jco_new (String, "4");
  void *c = jco_new (type, String, one, two, three, 0);

  ck_assert_int_eq (collection_size (c), 3);
  ck_assert (collection_contains (c, one));
  ck_assert (collection_contains (c, two));
  ck_assert (collection_contains (c, three));
  ck_assert (!collection_contains (c, four));
  
  ck_assert (mutable_collection_remove (c, two));
  ck_assert (!mutable_collection_remove (c, four));

  ck_assert_int_eq (collection_size (c), 2);
  ck_assert (collection_contains (c, one));
  ck_assert (!collection_contains (c, two));
  ck_assert (collection_contains (c, three));
  ck_assert (!collection_contains (c, four));
 
  jco_unref (one);
  jco_unref (two);
  jco_unref (three);
  jco_unref (four);
  jco_unref (c);
}
END_TEST

START_TEST (check_mutable_collection_remove_all)
{
  void *one = jco_new (String, "1");
  void *two = jco_new (String, "2");
  void *three = jco_new (String, "3");
  void *c = jco_new (type, String, one, two, three, 0);
  void *cc = jco_new (type, String, one, two, 0);

  ck_assert_int_eq (collection_size (c), 3);
  ck_assert (collection_contains (c, one));
  ck_assert (collection_contains (c, two));
  ck_assert (collection_contains (c, three));
  
  ck_assert (mutable_collection_remove_all (c, cc));

  ck_assert_int_eq (collection_size (c), 1);
  ck_assert (!collection_contains (c, one));
  ck_assert (!collection_contains (c, two));
  ck_assert (collection_contains (c, three));
 
  jco_unref (one);
  jco_unref (two);
  jco_unref (three);
  jco_unref (c);
  jco_unref (cc);
}
END_TEST

START_TEST (check_mutable_collection_retain_all)
{
  void *one = jco_new (String, "1");
  void *two = jco_new (String, "2");
  void *three = jco_new (String, "3");
  void *c = jco_new (type, String, one, two, three, 0);
  void *cc = jco_new (type, String, one, two, 0);

  ck_assert_int_eq (collection_size (c), 3);
  ck_assert (collection_contains (c, one));
  ck_assert (collection_contains (c, two));
  ck_assert (collection_contains (c, three));
  
  ck_assert (mutable_collection_retain_all (c, cc));

  ck_assert_int_eq (collection_size (c), 2);
  ck_assert (collection_contains (c, one));
  ck_assert (collection_contains (c, two));
  ck_assert (!collection_contains (c, three));
 
  jco_unref (one);
  jco_unref (two);
  jco_unref (three);
  jco_unref (c);
  jco_unref (cc);
}
END_TEST

int
check_mutable_collection (struct Class const *const class)
{
  jco_preconditions_check_not_null (class);
  type = class;

  jco_init_string();

  int number_failed = check_collection (type);
  Suite *s = suite_create ("MutableCollection");
  
  void *name = jco_to_string (class);
  TCase *tc = tcase_create (jco_string_to_cstring (name));
  jco_unref (name);

  tcase_add_test (tc, check_new_mutable_collection);

  tcase_add_test (tc, check_mutable_collection_add);
  tcase_add_test (tc, check_mutable_collection_add_all);
  tcase_add_test (tc, check_mutable_collection_clear);
  tcase_add_test (tc, check_mutable_collection_remove);
  tcase_add_test (tc, check_mutable_collection_remove_all);
  tcase_add_test (tc, check_mutable_collection_retain_all);

  suite_add_tcase (s, tc);

  SRunner *sr = srunner_create (s);

  srunner_set_fork_status (sr, CK_NOFORK);
  srunner_run_all (sr, CK_VERBOSE);
  number_failed = srunner_ntests_failed (sr);
  srunner_free (sr);

  return number_failed;
}
