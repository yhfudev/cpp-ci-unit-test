#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.60])
AC_INIT(ciut, 1.0.0, yhfudev@gmail.com)
AC_CONFIG_SRCDIR([include/ciut.h])
AC_CONFIG_HEADERS([config.h])
AM_INIT_AUTOMAKE(-Wall subdir-objects parallel-tests color-tests)

# Specify the directory of additional local Autoconf macros.
AC_CONFIG_MACRO_DIR([m4])

#magic for conditional check in Makefile:
MK=''; AC_SUBST(MK)
SED=sed

# Checks for programs.
AC_PROG_AWK
AC_PROG_CC
AC_PROG_CXX
AC_PROG_INSTALL
AC_PROG_MAKE_SET

# Checks for libraries.

AX_VALGRIND_DFLT([sgcheck], [off])
AX_VALGRIND_CHECK

# debug
AC_ARG_ENABLE([debug],
	AS_HELP_STRING([--enable-debug],[Compile the debug version (default: disabled)]),
	[enable_debug=$enableval],
	[enable_debug=no])
AM_CONDITIONAL([DEBUG], [test $enable_debug = "yes"])

# coverage
AC_ARG_ENABLE([coverage],
    AS_HELP_STRING([--enable-coverage],[Enable lcov coverage report generation (default: disabled)]),
    [enable_coverage=$enableval],
    [enable_coverage=no])
if test "x$enable_coverage" = "xyes"; then
    AC_PATH_PROG([LCOV], [lcov], [], [$PATH:/bin:/usr/bin:/usr/local/bin])
    if test x$LCOV = x; then
        AC_MSG_ERROR([lcov not found])
    fi
    AC_PATH_PROG([GENHTML], [genhtml], [], [$PATH:/bin:/usr/bin:/usr/local/bin])
    if test x$GENHTML = x; then
        AC_MSG_ERROR([genhtml not found])
    fi

    COVERAGE_CFLAGS=
    COVERAGE_LDFLAGS=

    # llvm native coverage mode:
    #AS_CASE([${CC}], [*clang*],
    #  [AX_CHECK_COMPILE_FLAG([-fprofile-instr-generate],
    #  [COVERAGE_CFLAGS="$COVERAGE_CFLAGS -fprofile-instr-generate"])])
    #AS_CASE([${CC}], [*clang*],
    #  [AX_CHECK_COMPILE_FLAG([-fcoverage-mapping],
    #  [COVERAGE_CFLAGS="$COVERAGE_CFLAGS -fcoverage-mapping"])])

    # gcov or llvm compabililty coverage mode:
    AS_CASE([${CC}], [*],
      [AX_CHECK_COMPILE_FLAG([-fprofile-arcs],
      [COVERAGE_CFLAGS="$COVERAGE_CFLAGS -fprofile-arcs"])])
    AS_CASE([${CC}], [*],
      [AX_CHECK_COMPILE_FLAG([-ftest-coverage],
      [COVERAGE_CFLAGS="$COVERAGE_CFLAGS -ftest-coverage"])])
    AS_CASE([${CC}], [*],
      [AX_CHECK_COMPILE_FLAG([--coverage],
      [COVERAGE_CFLAGS="$COVERAGE_CFLAGS --coverage"])])

    AS_CASE([${CC}], [*],
      [AX_CHECK_LINK_FLAG([-fprofile-arcs],
      [COVERAGE_LDFLAGS="$COVERAGE_LDFLAGS -fprofile-arcs"])])
    AS_CASE([${CC}], [*],
      [AX_CHECK_LINK_FLAG([-ftest-coverage],
      [COVERAGE_LDFLAGS="$COVERAGE_LDFLAGS -ftest-coverage"])])
    AS_CASE([${CC}], [*],
      [AX_CHECK_LINK_FLAG([--coverage],
      [COVERAGE_LDFLAGS="$COVERAGE_LDFLAGS --coverage"])])

    AC_SUBST(COVERAGE_CFLAGS)
    AC_SUBST(COVERAGE_LDFLAGS)

    CPPFLAGS="${COVERAGE_CFLAGS} ${CPPFLAGS}"
    LDFLAGS="${COVERAGE_LDFLAGS} ${LDFLAGS}"

    AC_MSG_NOTICE([coverage enabled, adding "-g -O0" to CFLAGS])
    #CFLAGS="${CFLAGS} -g -O0"
    enable_debug=yes
    AC_SUBST(enable_debug)
fi
AM_CONDITIONAL([COVERAGE], [test x$enable_coverage = xyes])

if test "x$enable_debug" = "xyes"  || "x$enable_coverage" = "xyes" ; then
  changequote({,})
  CFLAGS=`echo "$CFLAGS" | $SED -e 's/-O[0-9s]*//g'`
  CXXFLAGS=`echo "$CXXFLAGS" | $SED -e 's/-O[0-9s]*//g'`
  CPPFLAGS=`echo "$CPPFLAGS" | $SED -e 's/-O[0-9s]*//g'`
  LDFLAGS=`echo "$LDFLAGS" | $SED -e 's/-O[0-9s]*//g'`
  CFLAGS=`echo "$CFLAGS" | $SED -e 's/ -g //g'`
  CXXFLAGS=`echo "$CXXFLAGS" | $SED -e 's/ -g //g'`
  CPPFLAGS=`echo "$CPPFLAGS" | $SED -e 's/ -g //g'`
  LDFLAGS=`echo "$LDFLAGS" | $SED -e 's/ -g //g'`
  changequote([,])
  dnl add -O0 only if GCC or ICC is used
  if test "$GCC" = "yes" || test "$ICC" = "yes"; then
    CFLAGS="$CFLAGS -g -O0 -Wall"
    CXXFLAGS="$CXXFLAGS -g -O0 -Wall"
    CPPFLAGS="$CPPFLAGS -g -O0 -Wall"
    #LDFLAGS="$LDFLAGS -g -O0 -Wall"
  fi
fi


dnl Disable doc generation with doxygen option
AC_ARG_ENABLE(doxygen-doc,
              [AC_HELP_STRING([--disable-doxygen-doc],
                              [disable document generation with doxygen])],
              , enable_doxygen_doc="yes")
if test "x$enable_doxygen_doc" = "xyes"; then
  AC_CHECK_PROG(DOXYGEN,doxygen,doxygen,no)
  if test "x$DOXYGEN" = "xno"; then
    enable_doxygen_doc="no"
  fi
fi
dnl where to install the doxygen-generated HTML doc
AC_ARG_WITH(html-docdir,
            [AC_HELP_STRING([--with-html-docdir=DIR],
                            [where to install the doxyten-generated HTML doc [PREFIX/share/doc/ciut/html]])],
            [htmldocdir="$withval"], [htmldocdir=\$\{prefix\}/share/doc/ciut/html])
AC_SUBST(htmldocdir)
AM_CONDITIONAL(ENABLE_DOXYGEN_DOC,test "x$enable_doxygen_doc" = "xyes")


# Checks for header files.
AC_CHECK_HEADERS([stddef.h string.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_C_INLINE
AC_TYPE_SIZE_T

# Checks for library functions.

AC_CONFIG_FILES([Makefile
                 doc/Makefile
                 doc/Doxyfile
                 examples/Makefile
                ])
AC_OUTPUT
