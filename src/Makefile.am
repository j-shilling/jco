lib_LTLIBRARIES = libobject.la
libobject_la_SOURCES = object.c \
		       class.c \
		       string.c \
		       base-classes.c \
		       logger.c \
		       memory.c \
		       iterator.c \
		       collection.c \
		       mutablecollection.c \
		       list.c \
		       immutablearraylist.c \
		       arraylist.c
libobject_la_CFLAGS = ${CFLAGS} -I. -I../include

bin_PROGRAMS=test
test_SOURCES=main.c

test_LDADD=libobject.la
test_CFLAGS =${CFLAGS} -I. -I../include
