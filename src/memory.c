#include <stdlib.h>
#include <errno.h>
#include <string.h>

#include <jco/jco.h>

void *
jco_malloc (const size_t size)
{
  void *ret = malloc (size);
  if (NULL == ret)
    {
      logger_logf (SEVERE, "Could not allocate block; %s", strerror (errno));
      exit (EXIT_FAILURE);
    }
  return ret;
}

void
jco_free (const void *ptr)
{
  if (ptr)
    free((void *)ptr);
}

void *
jco_calloc (const size_t nmemb, const size_t size)
{
  void *ret = calloc (nmemb, size);
    if (NULL == ret)
      {
        logger_logf (SEVERE, "Could not allocate block; %s", strerror (errno));
        exit (EXIT_FAILURE);
      }
    return ret;
}

void *
jco_realloc (const void *ptr, const size_t size)
{
  void *ret = realloc ((void *)ptr, size);
    if (NULL == ret)
      {
        logger_logf (SEVERE, "Could not allocate block; %s", strerror (errno));
        exit (EXIT_FAILURE);
      }
    return ret;
}
