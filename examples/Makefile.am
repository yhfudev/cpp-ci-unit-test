
AM_LDFLAGS=
AM_CFLAGS=
AM_CPPFLAGS= -DCIUT_ENABLED=1
EXTRA_DIST=

if DEBUG
# use "valgrind --tool=memcheck --leak-check=yes" to check memory leak, MemWatch will drag the program.
#DEFS+=-DMEMWATCH=1
DEFS+=-DDEBUG=1
AM_CFLAGS+=-g  # -Werror -Wno-initializer-overrides # -Wall

else
AM_CFLAGS+=-O3 # -Werror -Wno-initializer-overrides # -Wall
endif

AM_CFLAGS+= \
    -I$(top_srcdir)/src/ \
    -I$(top_srcdir)/include/ \
    -I$(top_builddir)/include/ \
    $(NULL)

AM_CPPFLAGS+= $(AM_CFLAGS)

AM_LDFLAGS += \
    $(NULL)

DEFS+= \
    `getconf LFS_CFLAGS` \
    `getconf LFS64_CFLAGS` \
    -D_GNU_SOURCE=1 \
    -D_FILE_OFFSET_BITS=64 \
    -D_LARGEFILE64_SOURCE=1 \
    -DHAVE_MMAP64=1 \
    $(NULL)

DOCUMENT_FOLDER         ?= $(datadir)/doc/@PACKAGE@/

#noinst_PROGRAMS=ciutexc ciutexcpp
TESTS=ciutexc ciutexcpp
check_PROGRAMS=ciutexc ciutexcpp
#bin_PROGRAMS=ciutexc ciutexcpp


# force the autoconf use g++ to link the c program with c++ lib
dummy.cpp:
	touch $@
clean-local-dummy:
	-rm -rf dummy.cpp

#ciutexc.c:
#	echo -e "#define CIUT_PLACE_MAIN 1\n#include <ciut.h>\nint main(int argc, char * argv[]) { return ciut_main(argc, argv); }" > $@
#clean-local-check:
#	-rm -rf ciutexc.c

clean-local: clean-local-dummy #clean-local-check


ciutexc_SOURCES= \
    ciutexc.c \
    test-lang.c \
    test-sigsegv.c \
    $(NULL)
ciutexc_LDADD = #-lm
ciutexc_CFLAGS = $(AM_CFLAGS) -DDOCUMENT_FOLDER=\"$(DOCUMENT_FOLDER)\"
ciutexc_LDFLAGS = $(AM_LDFLAGS)

ciutexcpp_SOURCES= \
    ciutexcpp.cpp \
    test-lang.c \
    test-sigsegv.c \
    test-exception.cpp \
    test-library.cpp \
    $(NULL)
ciutexcpp_CXXFLAGS = -std=c++11 $(AM_CPPFLAGS) -DDOCUMENT_FOLDER=\"$(DOCUMENT_FOLDER)\"
ciutexcpp_LDFLAGS = $(AM_LDFLAGS)

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


