/*
 * This file is part of JCO
 *
 * JCO is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * JCO is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with JCO.  If not, see <http://www.gnu.org/licenses/>.
 */

/**
 * @file class.h
 * @author Jake Shilling
 * @brief Prototypes for functions relating to the Class structure.
 */

#ifndef __CLASS_H__
#define __CLASS_H__

#include <stdbool.h>
#include <stdarg.h>
#include <stdlib.h>

#include <jco/base/logger.h>
#include <jco/base/object.h>

/** Generate the bulk of a selector method. */
#define DEFINE_SELECTOR(name, default_method, _self, ...) 			\
      const struct Class *class = jco_class_of (_self); 			\
      if (!class) 								\
	{									\
	  jco_log (SEVERE, "This object does not have a class.");		\
	  exit (EXIT_FAILURE);							\
	}									\
										\
      *(Method *)&default_method = 						\
	jco_class_implements_method (class, (Selector) name) ?			\
	jco_class_get_method (class, (Selector) name) : (Method) default_method;\
										\
      if (!default_method)							\
	{									\
	  jco_log (SEVERE, "%O does not implement " #name, class);		\
	  exit (EXIT_FAILURE);							\
	}									\
										\
      return default_method (_self, ##__VA_ARGS__);


/** Type used to describe a Class */
struct Class;
/** Reference to instance of the Class class */
extern const struct Class *const Class;

/** Convenience name used to mark a method in a class */
typedef void (*Method)(void);
/** Convenience name used to make a function used to find a virtual method. */
typedef void (*Selector)(void);

/** Return the size of a given class / object */
size_t jco_size_of (void const *const _self);
/** Get the super class of a given class / object */
struct Class *jco_super (void const *const _self);

/** Returns true if the given class / object implements a given method. */
bool jco_class_implements_method (void const *const _self,
			      Selector const selector);
/** Returns true if the given class / object implements all given methods.
  The list of methods should end with a NULL value. */
bool jco_class_implements_all (void const *const _self,
			   ...);
/** Gets the method for a given class / object which overrides the given
  selector. */
Method jco_class_get_method (void const *const _self,
			 Selector const selector);

/** Call the objects constructor */
void *jco_construct (void *_self, va_list *app);
/** Call the object's super class' constructor */
void *jco_super_construct (const struct Class *const class,
		       void *_self, va_list *app);

/** Call the object's destructor */
void *jco_destruct (void *_self);
/** Call the object's super class' destructor */
void *jco_super_destruct (const struct Class *const class,
		      void *_self);

/** Convert the object to a string */
struct String *jco_to_string (void const *const _self);
/** Check if the two objects are equal */
bool jco_equals (void const *const _self, void const *const o);
/** Produce a hash code for the given object. */
int jco_hash_code (void const *const _self);

#endif /* __CLASS_H__ */
