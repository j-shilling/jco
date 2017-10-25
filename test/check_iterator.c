#include <check.h>

#include <jco/jco.h>
#include <jco/api/iterator.h>
#include <jco/api/collection.h>
#include <jco/api/arraylist.h>

static struct Class const *type = NULL;

START_TEST (check_is_iterator)
{
  void *c = jco_new (type, String, 0);
  void *it = collection_iterator (c);

  ck_assert (is_iterator (it));
  ck_assert (!iterator_has_next (it));

  jco_unref (it);
  jco_unref (c);
}
END_TEST

START_TEST (check_iterator_has_next)
{
  void *one = jco_new (String, "one");
  void *two = jco_new (String, "two");
  void *three = jco_new (String, "three");

  void *c = jco_new (type, String, one, two, three, 0);
  void *it = collection_iterator (c);

  for (int i = 0; i < collection_size (c); i++)
    {
      ck_assert (iterator_has_next (it));
      jco_unref (iterator_next(it));
    }

  ck_assert (!iterator_has_next (it));

  jco_unref (one);
  jco_unref (two);
  jco_unref (three);
  jco_unref (c);
  jco_unref (it);
}
END_TEST

START_TEST (check_iterator_next)
{
  void *one = jco_new (String, "one");
  void *two = jco_new (String, "two");
  void *three = jco_new (String, "three");

  void *c = jco_new (type, String, one, two, three, 0);
  void *it = collection_iterator (c);
  void *found = jco_new (ArrayList, String, 0);

  for (int i = 0; i < collection_size (c); i++)
    {
      void *item = iterator_next(it);

      ck_assert (item);
      ck_assert (!collection_contains (found, item));

      mutable_collection_add (found, item);
      jco_unref (item);
    }

  jco_unref (one);
  jco_unref (two);
  jco_unref (three);
  jco_unref (c);
  jco_unref (it);
}
END_TEST

int
check_iterator (struct Class const *const class)
{
  jco_preconditions_check_not_null (class);
  jco_preconditions_check_arg (
    jco_class_implements_method (class, (Selector) collection_iterator));

  type = class;

  jco_init_string();

  Suite *s = suite_create ("Iterator");
  
  void *name = jco_to_string (class);
  TCase *tc = tcase_create (jco_string_to_cstring(name));
  jco_unref (name);

  tcase_add_test (tc, check_is_iterator);
  tcase_add_test (tc, check_iterator_has_next);
  tcase_add_test (tc, check_iterator_next);

  suite_add_tcase (s, tc);

  SRunner *sr = srunner_create (s);

  srunner_set_fork_status (sr, CK_NOFORK);
  srunner_run_all (sr, CK_VERBOSE);
  int number_failed = srunner_ntests_failed (sr);
  srunner_free (sr);

  return number_failed;
}
