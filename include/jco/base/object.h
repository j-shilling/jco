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
 * @file object.h
 * @author Jake Shilling
 * @brief declare the Object class
 */

#ifndef __OBJECT_H__
#define __OBJECT_H__

#include <stdbool.h>
#include <stdarg.h>

#include <jco/base/class.h>

/**
 * A struct to hold an instance of an Object.
 */
struct Object;

/**
 * The instance of the Object class.
 */
extern const struct Class *const Object;

/**
 * @brief Returns a new instance of an Object.
 *
 * This is a varadic function. After work common to all
 * object types has been completed, all parameters except
 * for class are pased to the object's constructor.
 *
 * @param class	  The class of the object to be returned
 */
void *jco_new (struct Class const *const class, ...);

/**
 * @brief Destroy an instance of an object.
 *
 * This function first calls the object's destructor
 * and then frees the allocated memory. If the destructor
 * returns NULL, then nothing will be done by jco_delete.
 */
void  jco_delete (void *const _self);

/**
 * @brief Get the class of an instance of an object.
 */
struct Class const*jco_class_of (void const *const _self);

/**
 * @brief Returns true if the given parameter points to
 *	  an instance of Object.
 */
bool jco_is_object (void const *const _self);
/**
 * @brief Tests whether a pointer refers to an object
 *        of a given type, or of a subtype of the specified
 *        class.
 */
bool jco_is_descendant (void const *const _self,
			struct Class const *const class);
/**
 * @brief Tests whether a pointer refers to an object
 *        of exactly the specified class.
 */
bool jco_is (void const *const _self,
	     struct Class const *const class);

/**
 * @brief Terminates the program if the given pointer cannot
 *        be cast to a given type.
 */
void *jco_cast (void const *const _self, struct Class const *const class);


/**
 * @brief Informs the garbage collector that we are creating
 *	  a new reference to an instance of an object.
 * @returns   A copy of the pointer it was given.
 */
void *jco_ref (void const *const _self);

/**
 * @brief Informs the garbage collector that we are done with
 *        this pointer to an object reference.
 * @returns	A copy of the pointer it was given.
 */
void *jco_unref (void const *const _self);

#endif /* __OBJECT_H__ */
