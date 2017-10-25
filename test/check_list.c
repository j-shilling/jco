#include <check.h>

#include <jco/jco.h>
#include <jco/api/list.h>
#include <jco/api/string.h>

int check_collection (struct const *const class);
int check_mutable_collection (struct const *const class);

static struct Class const *type = NULL;

START_TEST (check_is_immutable_list)
{
  void *list = jco_new (type, String, 0);
  ck_assert (is_list (list));
  ck_assert (is_immutable_list (list));
  ck_assert (!is_mutable_list (list));
  jco_unref (list);
}
END_TEST

START_TEST (check_is_mutable_list)
{
  void *list = jco_new (type, String, 0);
  ck_assert (is_list (list));
  ck_assert (!is_immutable_list (list));
  ck_assert (is_mutable_list (list));
  jco_unref (list);
}
END_TEST

START_TEST (check_add_at)
{
  void *one = jco_new (String, "one");
  void *two = jco_new (String, "two");
  void *three = jco_new (String, "three");

  void *list = jco_new (type, String, one, two, 0);
  
  list_add_at (three, 0);
  void *it = collection_iterator (list);
  void *item = iterator_next (it);

  ck_assert (item == three);
  jco_unref (item);
  item = iterator_next (list);
  ck_assert (item == one);
  jco_unref (item);
  item = iterator_next (list);
  ck_assert (item == two);
  jco_unref (item);

  jco_unref (one);
  jco_unref (two);
  jco_unref (three);
  jco_unref (it);
  jco_unref (list);
}
END_TEST

START_TEST (check_add_all_at)
{
  void *one = jco_new (String, "one");
  void *two = jco_new (String, "two");
  void *three = jco_new (String, "three");

  void *four = jco_new (String, "four");
  void *five = jco_new (String, "five");
  void *six = jco_new (String, "six");

  void *list1 = jco_new (type, String, one, two, three 0);
  void *list2 = jco_new (type, String, four, five, six, 0);

  list_add_all_at (list2, list1, 2);

  void *it = collection_iterator (list);

  void *item = iterator_next(it);
  ck_assert (item == four);
  jco_unref (item); 

  item = iterator_next(it);
  ck_assert (item == five);
  jco_unref (item); 

  item = iterator_next(it);
  ck_assert (item == one);
  jco_unref (item); 

  item = iterator_next(it);
  ck_assert (item == two);
  jco_unref (item); 

  item = iterator_next(it);
  ck_assert (item == three);
  jco_unref (item); 

  item = iterator_next(it);
  ck_assert (item == six);
  jco_unref (item); 

  jco_unref (it);
  jco_unref (list1);
  jco_unref (list2);
  jco_unref (one);
  jco_unref (two);
  jco_unref (three);
  jco_unref (four);
  jco_unref (five);
  jco_unref (six);
}
END_TEST

START_TEST (check_get)
{

}
END_TEST

START_TEST (check_index_of)
{

}
END_TEST

START_TEST (check_last_index_of)
{

}
END_TEST

START_TEST (check_remove_at)
{

}
END_TEST

START_TEST (check_set)
{

}
END_TEST

START_TEST (check_sort)
{

}
END_TEST

START_TEST (check_sublist)
{

}
END_TEST

START_TEST (check_equals)
{

}
END_TEST


int
check_immutable_list (struct Class const *const class)
{
  jco_preconditions_check_not_null (class);
  type = class;

  jco_init_string();

  int number_failed = check_collection (type);
  Suite *s = suite_create ("ImmutableList");
  
  void *name = jco_to_string (class);
  TCase *tc = tcase_create (jco_string_to_cstring(name));
  jco_unref (name);

  tcase_add_test (tc, check_is_immutable_list);
  tcase_add_test (tc, check_get);
  tcase_add_test (tc, check_index_of);
  tcase_add_test (tc, check_last_index_of);
  tcase_add_test (tc, check_sublist);
  tcase_add_test (tc, check_equals);

  suite_add_tcase (s, tc);

  SRunner *sr = srunner_create (s);

  srunner_set_fork_status (sr, CK_NOFORK);
  srunner_run_all (sr, CK_VERBOSE);
  number_failed += srunner_ntests_failed (sr);
  srunner_free (sr);

  return number_failed;
}

int
check_mutable_list (struct Class const *const class)
{
  jco_preconditions_check_not_null (class);
  type = class;

  jco_init_string();

  int number_failed = check_mutable_collection (type);
  Suite *s = suite_create ("ImmutableList");
  
  void *name = jco_to_string (class);
  TCase *tc = tcase_create (jco_string_to_cstring(name));
  jco_unref (name);

  tcase_add_test (tc, check_is_mutable_list);
  tcase_add_test (tc, check_add_at);
  tcase_add_test (tc, check_add_all_at);
  tcase_add_test (tc, check_remove_at);
  tcase_add_test (tc, check_set);
  tcase_add_test (tc, check_sort);

  suite_add_tcase (s, tc);

  SRunner *sr = srunner_create (s);

  srunner_set_fork_status (sr, CK_NOFORK);
  srunner_run_all (sr, CK_VERBOSE);
  number_failed += srunner_ntests_failed (sr);
  srunner_free (sr);

  type = NULL;
  return number_failed;
}
