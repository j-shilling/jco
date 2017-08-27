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
      jco_object_constructor, jco_object_destructor, jco_object_equals, jco_object_to_string,
      jco_object_hash_code
    },

    {
      {
	  MAGIC_NUMBER, object + 1
      },
      "Class", object, sizeof(struct Class), NULL,
      class_constructor, class_destructor, jco_object_equals, class_to_string,
      jco_object_hash_code
    }
 };

const struct Class *const Object = object;
const struct Class *const Class = object + 1;
