#ifndef SRC_INTERFACE_PRIV_H_
#define SRC_INTERFACE_PRIV_H_

#include <object-priv.h>

struct Interface
{
  const struct Object _;

  const char *name;
  const struct Interface **extends;
};

#endif /* SRC_INTERFACE_PRIV_H_ */
