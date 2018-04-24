
AM_LDFLAGS=
AM_CFLAGS=
EXTRA_DIST=

if DEBUG
# use "valgrind --tool=memcheck --leak-check=yes" to check memory leak, MemWatch will drag the program.
#DEFS+=-DMEMWATCH=1
DEFS+=-DDEBUG=1
AM_CFLAGS +=-g -O0 # -Werror -Wno-initializer-overrides # -Wall

else
AM_CFLAGS +=-O3 # -Werror -Wno-initializer-overrides # -Wall
endif


EXT_FLAGS=
#@MK@GITNUMTMP=$(shell cd "$(top_srcdir)"; echo $$(git describe --tags|sed -r 's|v?(.+)|\1|'|sed 's|-|+|g'); cd - > /dev/null )
@MK@GITNUMTMP=$(shell cd "$(top_srcdir)"; A=$$(git show | head -n 1 | awk '{print $$2}'); echo $${A:0:7}; cd - > /dev/null )
#@MK@SVNNUMTMP=$(shell cd "$(top_srcdir)"; LC_ALL=C svn info | grep -i Revision | awk '{print $$2}'; cd - > /dev/null )
#@MK@ifeq ($(SVNNUMTMP),)
#EXT_FLAGS+= -DSVN_VERSION='"${GITNUMTMP}"'
#@MK@else
#EXT_FLAGS+= -DSVN_VERSION='"${SVNNUMTMP}"'
#@MK@endif
@MK@ifeq ($(GITNUMTMP),)
@MK@else
EXT_FLAGS+= -DSVN_VERSION='"${GITNUMTMP}"'
@MK@endif
DEFS+=$(EXT_FLAGS)


# add "VALGRIND = valgrind --gen-suppressions=all" in your makefile to get the content of .supp
# default valgrind supp file: /usr/lib/valgrind/default.supp
@VALGRIND_CHECK_RULES@
VALGRIND_SUPPRESSIONS_FILES = ciut-valgrind-test.supp
EXTRA_DIST += ciut-valgrind-test.supp

ciut-valgrind-test.supp:
	touch $@



AM_CFLAGS+= \
    -I$(top_srcdir)/src/ \
    -I$(top_srcdir)/include/ \
    -I$(top_builddir)/include/ \
    $(NULL)

AM_LDFLAGS += \
    $(NULL)

DEFS += \
    `getconf LFS_CFLAGS` \
    `getconf LFS64_CFLAGS` \
    -D_GNU_SOURCE=1 \
    -D_FILE_OFFSET_BITS=64 \
    -D_LARGEFILE64_SOURCE=1 \
    -DHAVE_MMAP64=1 \
    $(NULL)

DOCUMENT_FOLDER         ?= $(datadir)/doc/@PACKAGE@/

# force the autoconf use g++ to link the c program with c++ lib
dummy.cpp:
	touch $@
clean-local-dummy:
	-rm -rf dummy.cpp

#ciutexec.c:
#	echo -e "#define CIUT_PLACE_MAIN 1\n#include <ciut.h>\nint main(int argc, char * argv[]) { return ciut_main(argc, argv); }" > $@
#clean-local-check:
#	-rm -rf ciutexec.c

clean-local: clean-local-dummy #clean-local-check



AM_CFLAGS += -DCIUT_ENABLED=1

#noinst_PROGRAMS= ciutexecpp ciutexec
TESTS= ciutexecpp ciutexec
check_PROGRAMS= ciutexecpp ciutexec
#bin_PROGRAMS= ciutexecpp ciutexec

ciutexec_SOURCES= \
    ciutexec.c \
    test-sio.c \
    test-lang.c \
    test-sigsegv.c \
    $(NULL)
ciutexec_LDADD = #-lm
ciutexec_CFLAGS = $(AM_CFLAGS) -DDOCUMENT_FOLDER=\"$(DOCUMENT_FOLDER)\"
ciutexec_LDFLAGS = $(AM_LDFLAGS)

ciutexecpp_SOURCES= \
    ciutexecpp.cpp \
    test-sio.c \
    test-lang.c \
    test-sigsegv.c \
    test-exception.cpp \
    test-library.cpp \
    $(NULL)
ciutexecpp_CXXFLAGS = -std=c++11 $(AM_CFLAGS) -DDOCUMENT_FOLDER=\"$(DOCUMENT_FOLDER)\"
ciutexecpp_LDFLAGS = $(AM_LDFLAGS)

