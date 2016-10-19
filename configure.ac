#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ(2.59)
AC_INIT(libaeon, 0.16.21, earmbrust@users.sourceforge.net)
#AM_INIT_AUTOMAKE(libaeon, 0.16.21)
#This may make version management simpler. -ea
AM_INIT_AUTOMAKE([$PACKAGE_TARNAME], [$PACKAGE_VERSION])
AC_CONFIG_SRCDIR([config.h.in])
AC_CONFIG_HEADER([config.h])

# Checks for programs.
AC_PROG_CXX
AC_PROG_CC
AC_PROG_CPP
AC_PROG_INSTALL
AC_PROG_LN_S
AC_PROG_MAKE_SET
# Newer autotools complain about this?  Weird. -ea
#AC_PROG_LIBTOOL
LT_INIT
AC_PROG_RANLIB

# Configure for doxygen
# This isn't working just yet. Not sure why, more concerned with the library.
#DX_HTML_FEATURE(ON)
#DX_CHM_FEATURE(OFF)
#DX_CHI_FEATURE(OFF)
#DX_MAN_FEATURE(ON) 
#DX_RTF_FEATURE(OFF)
#DX_XML_FEATURE(OFF)
#DX_PDF_FEATURE(OFF) 
#DX_PS_FEATURE(OFF) 
#DX_INIT_DOXYGEN(libaeon, libaeon.DoxyFile, docs)



# Checks for libraries.

# Checks for header files.
AC_HEADER_STDC
AC_CHECK_HEADERS([arpa/inet.h netdb.h netinet/in.h string.h sys/socket.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_HEADER_STDBOOL
AC_C_CONST

AC_SUBST([AM_CFLAGS])
AC_SUBST([AM_CXXFLAGS])


# Checks for library functions.
AC_CHECK_FUNCS([gethostbyname inet_ntoa memset socket])

AC_CONFIG_FILES([Makefile
                 src/Makefile])
AC_OUTPUT
