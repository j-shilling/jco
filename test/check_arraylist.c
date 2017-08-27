#include <check.h>

#include <jco/jco.h>
#include <jco/api/arraylist.h>
#include <jco/api/string.h>

START_TEST (check_new_immutable_array_list)
{
  void *list = jco_new (ImmutableArrayList, String, 0);

  ck_assert_ptr_ne (list, 0);
  ck_assert (collection_is_empty (list));
  ck_assert (collection_content_type (list) == String);

  jco_unref (list);
}
END_TEST

START_TEST (check_new_immutable_array_list_with_items)
{
  void *one = jco_new (String, "1");
  void *two = jco_new (String, "2");
  void *three = jco_new (String, "3");
  void *list = jco_new (ImmutableArrayList, String, one, two, three, 0);

  ck_assert_ptr_ne (list, 0);
  ck_assert (!collection_is_empty (list));
  ck_assert_int_eq (collection_size (list), 3);

  void *it = collection_iterator (list);
  
  void *x = iterator_next (it);
  ck_assert_ptr_eq (x, one);
  jco_unref (x);
  x = iterator_next (it);
  ck_assert_ptr_eq (x, two);
  jco_unref (x);
  x = iterator_next (it);
  ck_assert_ptr_eq (x, three);
  jco_unref (x);

  jco_unref (one);
  jco_unref (two);
  jco_unref (three);
  jco_unref (list);
}
END_TEST

START_TEST (check_new_immutable_array_list_with_collection);
{
  void *one = jco_new (String, "1");
  void *two = jco_new (String, "2");
  void *three = jco_new (String, "3");
  void *src = jco_new (ImmutableArrayList, String, one, two, three, 0);
  void *list = jco_new (ImmutableArrayList, String, src, 0);

  ck_assert_ptr_ne (list, 0);
  ck_assert (!collection_is_empty (list));
  ck_assert_int_eq (collection_size (list), 3);

  void *it = collection_iterator (list);
  
  void *x = iterator_next (it);
  ck_assert_ptr_eq (x, one);
  jco_unref (x);
  x = iterator_next (it);
  ck_assert_ptr_eq (x, two);
  jco_unref (x);
  x = iterator_next (it);
  ck_assert_ptr_eq (x, three);
  jco_unref (x);

  jco_unref (one);
  jco_unref (two);
  jco_unref (three);
  jco_unref (src);
  jco_unref (list);
}
END_TEST

START_TEST (check_new_immutable_array_list_with_items_and_collection);
{
  void *one = jco_new (String, "1");
  void *two = jco_new (String, "2");
  void *three = jco_new (String, "3");
  void *src = jco_new (ImmutableArrayList, String, two, three, 0);
  void *list = jco_new (ImmutableArrayList, String, one, src, 0);

  ck_assert_ptr_ne (list, 0);
  ck_assert (!collection_is_empty (list));
  ck_assert_int_eq (collection_size (list), 3);

  void *it = collection_iterator (list);
  
  void *x = iterator_next (it);
  ck_assert_ptr_eq (x, one);
  jco_unref (x);
  x = iterator_next (it);
  ck_assert_ptr_eq (x, two);
  jco_unref (x);
  x = iterator_next (it);
  ck_assert_ptr_eq (x, three);
  jco_unref (x);

  jco_unref (one);
  jco_unref (two);
  jco_unref (three);
  jco_unref (src);
  jco_unref (list);
}
END_TEST

START_TEST (check_immutable_array_list_contains)
{
  void *one = jco_new (String, "1");
  void *two = jco_new (String, "2");
  void *three = jco_new (String, "3");
  void *list = jco_new (ImmutableArrayList, String, one, two, 0);

  ck_assert (collection_contains (list, one));
  ck_assert (collection_contains (list, two));
  ck_assert (!collection_contains (list, three));

  jco_unref (one);
  jco_unref (two);
  jco_unref (three);
  jco_unref (list);
}
END_TEST

START_TEST (check_immutable_array_list_contains_all)
{
  void *one = jco_new (String, "1");
  void *two = jco_new (String, "2");
  void *three = jco_new (String, "3");
  void *test = jco_new (ImmutableArrayList, String, one, two, 0);
  void *list = jco_new (ImmutableArrayList, String, one, two, three, 0);

  ck_assert (collection_contains_all (list, test));
  ck_assert (!collection_contains_all (test, list));

  jco_unref (one);
  jco_unref (two);
  jco_unref (three);
  jco_unref (list);
  jco_unref (test);
}
END_TEST

START_TEST (check_immutable_array_list_equals)
{
  void *one = jco_new (String, "1");
  void *two = jco_new (String, "2");
  void *three = jco_new (String, "3");

  void *list = jco_new (ImmutableArrayList, String, one, two, three, 0);
  void *yes = jco_new (ImmutableArrayList, String, one, two, three, 0);
  void *no = jco_new (ImmutableArrayList, String, three, two, one, 0);

  ck_assert (collection_equals (list, yes));
  ck_assert (!collection_equals (list, no));

  jco_unref (one);
  jco_unref (two);
  jco_unref (three);
  jco_unref (list);
  jco_unref (yes);
  jco_unref (no);
}
END_TEST


int
main (void)
{
  initString();
  initArrayList();

  Suite *s = suite_create ("ArrayList");
  TCase *tc = tcase_create ("Core");

  tcase_add_test (tc, check_new_immutable_array_list);
  tcase_add_test (tc, check_new_immutable_array_list_with_items);
  tcase_add_test (tc, check_new_immutable_array_list_with_collection);
  tcase_add_test (tc, check_new_immutable_array_list_with_items_and_collection);
  tcase_add_test (tc, check_immutable_array_list_contains);
  tcase_add_test (tc, check_immutable_array_list_contains_all);
  tcase_add_test (tc, check_immutable_array_list_equals);

  suite_add_tcase (s, tc);

  SRunner *sr = srunner_create (s);

  srunner_set_fork_status (sr, CK_NOFORK);
  srunner_run_all (sr, CK_VERBOSE);
  int number_failed = srunner_ntests_failed (sr);
  srunner_free (sr);

  return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
