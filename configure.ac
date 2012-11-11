# Process this file with autoconf to produce a configure script.
AC_INIT(configure.ac)
AC_CONFIG_SRCDIR(src/cmml.h)

AC_PREREQ(2.53)
AC_CANONICAL_TARGET

AM_INIT_AUTOMAKE(libcmml,0.9.4)
AM_CONFIG_HEADER(config.h)

#------------------------------------------------------------------------------------
# Rules for library version information:
#
#  1. Start with version information of `0:0:0' for each libtool library.
#  2. Update the version information only immediately before a public release of
#     your software. More frequent updates are unnecessary, and only guarantee
#     that the current interface number gets larger faster.
#  3. If the library source code has changed at all since the last update, then
#     increment revision (`c:r:a' becomes `c:r+1:a').
#  4. If any interfaces have been added, removed, or changed since the last update,
#     increment current, and set revision to 0.
#  5. If any interfaces have been added since the last public release, then increment
#     age.
#  6. If any interfaces have been removed since the last public release, then set age
#     to 0.

SHARED_VERSION_INFO="1:0:0"
SHLIB_VERSION_ARG=""

# Checks for programs.
AC_PROG_CC
AC_PROG_CC_STDC
AC_PROG_INSTALL
AC_PROG_LN_S
AC_PROG_MAKE_SET
AC_PROG_LIBTOOL

# Checks for typedefs, structures, and compiler characteristics.
AC_TYPE_SIZE_T
AC_C_CONST
AC_C_BIGENDIAN

dnl Add parameters for aclocal
AC_SUBST(ACLOCAL_AMFLAGS, "-I m4")

# Checks for header files.
AC_HEADER_STDC
AC_CHECK_HEADERS([fcntl.h stdlib.h string.h getopt.h unistd.h math.h])

# Checks for library functions.
AC_FUNC_MALLOC
AC_CHECK_FUNCS([strdup strtol])

# Overall configuration success flag
libcmml_config_ok=yes

dnl Use -Wall if we have gcc.
dnl changequote(,)dnl
if test "x$ac_cv_prog_gcc" = xyes ; then
  CFLAGS="$CFLAGS -Wall -g"
fi
dnl changequote([,])dnl

# include Version_script
case "$target_os" in
  darwin* | rhapsody*)
    dnl Disable -Wall and -pedantic for Apple Darwin/Rhapsody.
    dnl System headers on these systems are broken.
    temp_CFLAGS=`echo $CFLAGS | sed "s/-Wall -pedantic//"`
    CFLAGS=$temp_CFLAGS
    ;;
  linux* | solaris*)
    SHLIB_VERSION_ARG="-Wl,--version-script=Version_script"
    ;;
  *)
    ;;
esac

# Checks for doxygen
AC_CHECK_PROG(HAVE_DOXYGEN, doxygen, true, false)
AM_CONDITIONAL(HAVE_DOXYGEN,$HAVE_DOXYGEN)
if test $HAVE_DOXYGEN = "false"; then
             AC_MSG_WARN([*** doxygen not found, docs will not be built])
fi
 
# Checks for docbook
AC_CHECK_PROG(HAVE_DOCBOOKTOMAN, docbook-to-man, true, false)
AM_CONDITIONAL(HAVE_DOCBOOKTOMAN,$HAVE_DOCBOOKTOMAN)
AC_CHECK_PROG(HAVE_DOCBOOK2HTML, docbook2html, true, false)
AM_CONDITIONAL(HAVE_DOCBOOK2HTML,$HAVE_DOCBOOK2HTML)


# Check for valgrind
ac_enable_valgrind=no
AC_ARG_ENABLE(valgrind-testing,
     [  --enable-valgrind-testing     enable running of tests inside Valgrind ],
     [ ac_enable_valgrind=yes ], [ ac_enable_valgrind=no] )

if test "x${ac_enable_valgrind}" = xyes ; then
  if test "x${enable_shared}" = xyes ; then
    AC_MSG_WARN([
*** You have requested to test under valgrind, but have not disabled the
*** building of shared libraries. You usually need to do:
***   ./configure --disable-shared --enable-valgrind-testing
*** for Valgrind to produce useful output.])
  fi

  AC_CHECK_PROG(HAVE_VALGRIND, valgrind, yes, no)
  if test "x$HAVE_VALGRIND" = xyes ; then
    VALGRIND_ENVIRONMENT="valgrind -q --leak-check=yes --show-reachable=yes --num-callers=100"
    AC_SUBST(VALGRIND_ENVIRONMENT)
    TESTS_INFO="Test suite will be run under:
    ${VALGRIND_ENVIRONMENT}"
  else
    TESTS_INFO="(Valgrind not found)"
  fi
else
  TESTS_INFO="(Valgrind testing not enabled)"
fi

# check for getopt in a separate library
HAVE_GETOPT=no
AC_CHECK_LIB(getopt, getopt, HAVE_GETOPT="yes")
if test "x$HAVE_GETOPT" = xyes ; then
  GETOPT_LIBS="-lgetopt"
  AC_SUBST(GETOPT_LIBS)
fi

# check for getopt_long in standard library
HAVE_GETOPT_LONG=no
AC_CHECK_FUNC(getopt_long, HAVE_GETOPT_LONG="yes")
if test "x$HAVE_GETOPT_LONG" = xyes ; then
  AC_DEFINE(HAVE_GETOPT_LONG, [], [Define to 1 if you have the 'getop_long' function])
else
  # FreeBSD has a gnugetopt library
  AC_CHECK_LIB(gnugetopt, getopt_long, HAVE_GETOPT_lONG="yes")
  if test "x$HAVE_GETOPT_LONG" = xyes ; then
    AC_DEFINE(HAVE_GETOPT_LONG, [], 
      [Define to 1 if you have the 'getop_long' function])
    GETOPT_LONG_LIBS="-lgnugetopt"
    AC_SUBST(GETOPT_LONG_LIBS)
  fi
fi

# Checks for expat library
HAVE_EXPAT=no
AC_CHECK_LIB(expat, XML_ParserCreate, HAVE_EXPAT="yes")
if test "x$HAVE_EXPAT" = xyes ; then
  EXPAT_LIBS="-lexpat"
  AC_SUBST(EXPAT_LIBS)
else
  libcmml_config_ok="no"
fi

# configure complete
if test $libcmml_config_ok = no ; then

AC_MSG_RESULT([

*** $PACKAGE $VERSION: Automatic configuration FAILED.

*** The file config.log has full details.

*** The following required libraries are missing or
*** misconfigured on your system:
])

if test "x$HAVE_EXPAT" = xno ; then
  AC_MSG_RESULT([
      XML parsing library expat is missing
      available from: http://expat.sourceforge.net/
])
fi

AC_MSG_ERROR([
***
*** After fixing the above problems, you must run ./configure again.
***
])

else

AC_SUBST(SHLIB_VERSION_ARG)
AC_SUBST(SHARED_VERSION_INFO)

AC_OUTPUT([
Win32/config.h
doc/Doxyfile
doc/cmml-validate.1.sgml
doc/cmml-fix.1.sgml
doc/cmml-fortune.6.sgml
doc/cmml-timeshift.1.sgml
Makefile
src/Makefile
src/Version_script
tools/Makefile
examples/Makefile
tests/Makefile
doc/Makefile
cmml.pc])

AS_AC_EXPAND(LIBDIR, ${libdir})
AS_AC_EXPAND(INCLUDEDIR, ${includedir})
AS_AC_EXPAND(BINDIR, ${bindir})
AS_AC_EXPAND(DOCDIR, ${datadir}/doc)

AC_MSG_RESULT([
------------------------------------------------------------------------
  $PACKAGE $VERSION:  Automatic configuration OK.

  Tools:

    cmml-validate...for checking validity of a CMML file according 
                    to the DTD

    cmml-fix........for fixing a CMML file with sloppy errors and
                    returning a valid CMML file

    cmml-fortune....for creating a random valid CMML file

    cmml-timeshift..for shifting start/end time of clip tags


  Installation paths:

    libcmml: ..................... ${LIBDIR}
    Tools: ....................... ${BINDIR}
    C header files: .............. ${INCLUDEDIR}
    Documentation: ............... ${DOCDIR}/$PACKAGE

  Building:

    Type 'make' to compile $PACKAGE.

    Type 'make install' to install $PACKAGE.

    Type 'make check' to test $PACKAGE on the files *.cmml 
    contained in the tests directory.
    ${TESTS_INFO}

  Example programs will be built but not installed.
------------------------------------------------------------------------
])

fi
