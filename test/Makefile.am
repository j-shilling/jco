TESTS = check_jco
check_PROGRAMS = check_jco

check_jco_SOURCES = check_object.c \
						string.c

check_jco_CFLAGS = @CHECK_CFLAGS@ -I$(srcdir)/../include
check_jco_LDADD = @CHECK_LIBS@ $(srcdir)/../src/libjco.la
