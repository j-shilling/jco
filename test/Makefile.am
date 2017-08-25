TESTS = check_object
check_PROGRAMS = check_object

check_object_SOURCES = check_object.c \
						string.c

check_object_CFLAGS = @CHECK_CFLAGS@ -I$(srcdir)/../include
check_object_LDADD = @CHECK_LIBS@ $(srcdir)/../src/libobject.la