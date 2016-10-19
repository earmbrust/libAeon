#!/bin/bash
#######################################
# autogen.sh script
# Copyright 2006 (c) Elden Armbrust
#

WHICH_CMD=which
ACLOCAL_BIN=aclocal
AUTOMAKE_BIN=automake
AUTOCONF_BIN=autoconf
AUTOHEADER_BIN=autoheader
LIBTOOL_BIN=libtoolize
CAT_BIN=cat
GREP_BIN=grep

AUTOMAKE=`$WHICH_CMD $AUTOMAKE_BIN`
AUTOCONF=`$WHICH_CMD $AUTOCONF_BIN`
AUTOHEADER=`$WHICH_CMD $AUTOHEADER_BIN`
ACLOCAL=`$WHICH_CMD $ACLOCAL_BIN`
LIBTOOLIZE=`$WHICH_CMD $LIBTOOL_BIN`
CAT=`$WHICH_CMD $CAT_BIN`
GREP=`$WHICH_CMD $GREP_BIN`

if [ "$ACLOCAL" = "" ]; then
  echo "aclocal was not found on the system."
  exit
else
  echo "aclocal found as $ACLOCAL"
fi

if [ "$LIBTOOLIZE" = "" ]; then
  echo "libtool was not found on the system."
else
  echo "libtool found as $LIBTOOLIZE"
fi

if [ "$AUTOMAKE" = "" ]; then
  echo "automake was not found on the system."
  exit
else
  echo "automake found as $AUTOMAKE"
fi

if [ "$AUTOCONF" = "" ]; then
  echo "autoconf was not found on the system."
  exit
else
  echo "autoconf found as $AUTOCONF"

fi

if [ "$AUTOHEADER" = "" ]; then
  echo "autoheader was not found on the system."
  exit
else
  echo "autoheader found as $AUTOHEADER"
fi

echo "Proceeding with automatic generation..."


echo "Running aclocal..."
$ACLOCAL
echo "Running automake..."
$AUTOMAKE -a

if [ -e configure.in ]; then
  CONFIGURE=configure.in
fi

if [ -e configure.ac ]; then
  CONFIGURE=configure.ac
fi

if [ `$CAT $CONFIGURE | $GREP AC_PROG_LIBTOOL` = "AC_PROG_LIBTOOL" ]; then
  echo "Found libtool reference in $CONFIGURE."
  if [ "$LIBTOOLIZE" != "" ]; then
    echo "Running libtool..."
    $LIBTOOLIZE --force
  else
    echo "Error running libtool."
    exit
  fi
fi

echo "Running autoconf..."
$AUTOCONF
echo "Running autoheader..."
$AUTOHEADER

echo
echo "You may now run ./configure to configure libaeon."
echo