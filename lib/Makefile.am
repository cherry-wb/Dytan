pkglib_LTLIBRARIES = libitrace.la
libitrace_la_SOURCES = itrace.c decode.c ir.c
libitrace_la_CFLAGS = -Werror -Wall -Wextra --pedantic -std=c99
libitrace_la_CPPFLAGS = -I$(top_srcdir)/include
libitrace_la_LIBADD = libudis86/libudis86.la
