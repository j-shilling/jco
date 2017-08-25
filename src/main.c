#include <object/logger.h>

int main ()
{
  for (int i = 0; i < 10; i++)
    logger_logf (SEVERE, "%03d", i);
}
