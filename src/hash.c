#include <jco/jco.h>

#include <stdint.h>
#include <limits.h>
#include <string.h>

int
jco_hash_int (int const val)
{
  return val;
}

int
jco_hash_long_long (long long const val)
{
  return (int)(val % INT_MAX);
}

int
jco_hash_long_double (long double const val)
{
  long long l;
  memcpy (&l, &val, sizeof (long long));
  return jco_hash_long_long (l);
}

int
jco_hash_string (char const *const val)
{
  int h = 0;
  int len = strlen (val);
  char const *ptr = val;

  for (int i = 0; i < len; i ++)
    h = 31 * h + (int)(*(ptr++));

  return h;
}

int
jco_hash_object (void const *const val)
{
  return jco_hash_code (val);
}

int
jco_hash_ptr (void const *const val)
{
  if (jco_is_object (val))
    return jco_hash_object (val);
  else
    return (int) (long) val;
}
