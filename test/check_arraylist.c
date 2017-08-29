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
  ck_assert (is_collection (list));
  ck_assert (!is_mutable_collection (list));
  ck_assert (is_list (list));
  ck_assert (is_immutable_list (list));
  ck_assert (!is_mutable_list (list));

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

START_TEST (check_immutable_array_list_get)
{
  void *one = jco_new (String, "1");
  void *two = jco_new (String, "2");
  void *three = jco_new (String, "3");
  void *list = jco_new (ImmutableArrayList, String, one, two, three, 0);

  ck_assert_ptr_eq (one, jco_unref(list_get (list, 0)));
  ck_assert_ptr_eq (two, jco_unref(list_get (list, 1)));
  ck_assert_ptr_eq (three, jco_unref(list_get (list, 2)));

  jco_unref (one);
  jco_unref (two);
  jco_unref (three);
  jco_unref (list);
}
END_TEST

START_TEST (check_immutable_array_list_index_of)
{
  void *one = jco_new (String, "1");
  void *one1 = jco_new (String, "1");
  void *two = jco_new (String, "2");
  void *three = jco_new (String, "3");
  void *list = jco_new (ImmutableArrayList, String, one, two, one1, three, 0);

  ck_assert_int_eq (list_index_of (list, one), 0);
  ck_assert_int_eq (list_index_of (list, two), 1);
  ck_assert_int_eq (list_index_of (list, three), 3);

  jco_unref (one);
  jco_unref (one1);
  jco_unref (two);
  jco_unref (three);
  jco_unref (list);
}
END_TEST

START_TEST (check_immutable_array_list_last_index_of)
{
  void *one = jco_new (String, "1");
  void *one1 = jco_new (String, "1");
  void *two = jco_new (String, "2");
  void *three = jco_new (String, "3");
  void *list = jco_new (ImmutableArrayList, String, one, two, one1, three, 0);

  ck_assert_int_eq (list_last_index_of (list, one), 2);

  jco_unref (one);
  jco_unref (one1);
  jco_unref (two);
  jco_unref (three);
  jco_unref (list);
}
END_TEST

START_TEST (check_immutable_array_list_sublist)
{
  void *one = jco_new (String, "1");
  void *one1 = jco_new (String, "1");
  void *two = jco_new (String, "2");
  void *three = jco_new (String, "3");
  void *list = jco_new (ImmutableArrayList, String, one, two, one1, three, 0);
  void *sub = jco_new (ImmutableArrayList, String, one, two, 0);
  void *sublist = list_sublist (list, 0, 1);

  ck_assert (list_equals (sub, sublist));

  jco_unref (one);
  jco_unref (one1);
  jco_unref (two);
  jco_unref (three);
  jco_unref (list);
  jco_unref (sub);
  jco_unref (sublist);
}
END_TEST

START_TEST (check_new_array_list_with_items_and_collection);
{
  void *one = jco_new (String, "1");
  void *two = jco_new (String, "2");
  void *three = jco_new (String, "3");
  void *src = jco_new (ImmutableArrayList, String, two, three, 0);
  void *list = jco_new (ArrayList, String, one, src, 0);

  ck_assert_ptr_ne (list, 0);
  ck_assert (!collection_is_empty (list));
  ck_assert_int_eq (collection_size (list), 3);
  ck_assert (is_collection (list));
  ck_assert (is_list (list));
  ck_assert (is_mutable_list (list));
  ck_assert (!is_immutable_list (list));

  ck_assert_ptr_eq (jco_unref (list_get (list, 0)), one);
  ck_assert_ptr_eq (jco_unref (list_get (list, 1)), two);
  ck_assert_ptr_eq (jco_unref (list_get (list, 2)), three);

  jco_unref (one);
  jco_unref (two);
  jco_unref (three);
  jco_unref (src);
  jco_unref (list);
}
END_TEST

START_TEST (check_array_list_remove)
{
  void *one = jco_new (String, "1");
  void *two = jco_new (String, "2");
  void *three = jco_new (String, "3");
  void *four = jco_new (String, "4");
  void *list = jco_new (ArrayList, String, one, two, three, 0);
  
  ck_assert (mutable_collection_remove (list, two));
  ck_assert (!mutable_collection_remove (list, four));

  ck_assert_int_eq (collection_size (list), 2);

  ck_assert_ptr_eq (jco_unref (list_get (list, 0)), one);
  ck_assert_ptr_eq (jco_unref (list_get (list, 1)), three);

  jco_unref (one);
  jco_unref (two);
  jco_unref (three);
  jco_unref (four);
  jco_unref (list);
}
END_TEST

START_TEST (check_array_list_remove_at)
{
  void *one = jco_new (String, "1");
  void *two = jco_new (String, "2");
  void *three = jco_new (String, "3");
  void *list = jco_new (ArrayList, String, one, two, three, 0);
  
  ck_assert (list_remove_at (list, 1));

  ck_assert_int_eq (collection_size (list), 2);

  ck_assert_ptr_eq (jco_unref (list_get (list, 0)), one);
  ck_assert_ptr_eq (jco_unref (list_get (list, 1)), three);

  jco_unref (one);
  jco_unref (two);
  jco_unref (three);
  jco_unref (list);
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
  tcase_add_test (tc, check_immutable_array_list_get);
  tcase_add_test (tc, check_immutable_array_list_index_of);
  tcase_add_test (tc, check_immutable_array_list_last_index_of);
  tcase_add_test (tc, check_immutable_array_list_sublist);

  tcase_add_test (tc, check_new_array_list_with_items_and_collection);
  tcase_add_test (tc, check_array_list_remove);
  tcase_add_test (tc, check_array_list_remove_at);

  suite_add_tcase (s, tc);

  SRunner *sr = srunner_create (s);

  srunner_set_fork_status (sr, CK_NOFORK);
  srunner_run_all (sr, CK_VERBOSE);
  int number_failed = srunner_ntests_failed (sr);
  srunner_free (sr);

  return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
