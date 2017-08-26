#ifndef INCLUDE_OBJECT_API_STRING_H_
#define INCLUDE_OBJECT_API_STRING_H_

#include <stdbool.h>

#define string_append(x, y) _Generic ((y),  \
		   int: string_append_char, \
		  char: string_append_char, \
	     char *: string_append_cstring, \
       const char *: string_append_cstring, \
  const char *const: string_append_cstring, \
      default: string_append_object) (x, y)

#define string_ends_with(x, y) _Generic ((y),  \
		   int: string_ends_with_char, \
		  char: string_ends_with_char, \
	     char *: string_ends_with_cstring, \
       const char *: string_ends_with_cstring, \
  const char *const: string_ends_with_cstring, \
	default: string_ends_with_object) (x, y)

struct String;
extern const struct Class * String;

void initString();

const char *string_to_cstring (const void *_self);
void string_append_cstring (const void *_self, char *str);
void string_append_object (const void *_self, void *o);
void string_append_char (const void *_self, char ch);
unsigned int string_length(const void *_self);
bool string_ends_with_char (const void *_self, char ch);
bool string_ends_with_cstring (const void *_self, char *str);
bool string_ends_with_object (const void *_self, void *o);
char string_char_at (const void *_self, unsigned int index);

#endif /* INCLUDE_OBJECT_API_STRING_H_ */
