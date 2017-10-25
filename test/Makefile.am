TESTS = check_logger check_string check_arraylist check_hashset
check_PROGRAMS = check_logger check_string check_arraylist check_hashset

check_logger_SOURCES = check_logger.c

check_logger_CFLAGS = @CHECK_CFLAGS@ -I$(srcdir)/../include
check_logger_LDADD = @CHECK_LIBS@ $(srcdir)/../src/libjco.la

check_string_SOURCES = check_string.c

check_string_CFLAGS = @CHECK_CFLAGS@ -I$(srcdir)/../include
check_string_LDADD = @CHECK_LIBS@ $(srcdir)/../src/libjco.la

check_arraylist_SOURCES = check_arraylist.c \
			  check_iterator.c \
			  check_collection.c \
			  check_mutable_collection.c

check_arraylist_CFLAGS = @CHECK_CFLAGS@ -I$(srcdir)/../include
check_arraylist_LDADD = @CHECK_LIBS@ $(srcdir)/../src/libjco.la

check_hashset_SOURCES = check_hashset.c \
			check_iterator.c \
			check_collection.c \
			check_mutable_collection.c

check_hashset_CFLAGS = @CHECK_CFLAGS@ -I$(srcdir)/../include
check_hashset_LDADD = @CHECK_LIBS@ $(srcdir)/../src/libjco.la
