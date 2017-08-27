TESTS = check_logger check_string check_arraylist
check_PROGRAMS = check_logger check_string check_arraylist

check_logger_SOURCES = check_logger.c

check_logger_CFLAGS = @CHECK_CFLAGS@ -I$(srcdir)/../include
check_logger_LDADD = @CHECK_LIBS@ $(srcdir)/../src/libjco.la

check_string_SOURCES = check_string.c

check_string_CFLAGS = @CHECK_CFLAGS@ -I$(srcdir)/../include
check_string_LDADD = @CHECK_LIBS@ $(srcdir)/../src/libjco.la

check_arraylist_SOURCES = check_arraylist.c

check_arraylist_CFLAGS = @CHECK_CFLAGS@ -I$(srcdir)/../include
check_arraylist_LDADD = @CHECK_LIBS@ $(srcdir)/../src/libjco.la

