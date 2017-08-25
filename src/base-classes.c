#include <class-priv.h>
#include <object-priv.h>
#include <string-priv.h>
#include <hashmap-priv.h>

#include <stdlib.h>

static const struct Class object[] =
  {
    {
      {
	  MAGIC_NUMBER, object + 1
      },
      "Object", object, sizeof(struct Object), NULL,
      object_constructor, object_destructor, object_equals, object_to_string,
      object_hash_code
    },

    {
      {
	  MAGIC_NUMBER, object + 1
      },
      "Class", object, sizeof(struct Class), NULL,
      class_constructor, class_destructor, object_equals, class_to_string,
      object_hash_code
    }
 };

const struct Class *const Object = object;
const struct Class *const Class = object + 1;
