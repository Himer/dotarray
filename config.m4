dnl $Id$
dnl config.m4 for extension dotarray

dnl Comments in this file start with the string 'dnl'.
dnl Remove where necessary. This file will not work
dnl without editing.

dnl If your extension references something external, use with:

dnl PHP_ARG_WITH(dotarray, for dotarray support,
dnl Make sure that the comment is aligned:
dnl [  --with-dotarray             Include dotarray support])

dnl Otherwise use enable:

PHP_ARG_ENABLE(dotarray, whether to enable dotarray support,
dnl Make sure that the comment is aligned:
[  --enable-dotarray           Enable dotarray support])

if test "$PHP_DOTARRAY" != "no"; then
  dnl Write more examples of tests here...

  dnl # --with-dotarray -> check with-path
  dnl SEARCH_PATH="/usr/local /usr"     # you might want to change this
  dnl SEARCH_FOR="/include/dotarray.h"  # you most likely want to change this
  dnl if test -r $PHP_DOTARRAY/$SEARCH_FOR; then # path given as parameter
  dnl   DOTARRAY_DIR=$PHP_DOTARRAY
  dnl else # search default path list
  dnl   AC_MSG_CHECKING([for dotarray files in default path])
  dnl   for i in $SEARCH_PATH ; do
  dnl     if test -r $i/$SEARCH_FOR; then
  dnl       DOTARRAY_DIR=$i
  dnl       AC_MSG_RESULT(found in $i)
  dnl     fi
  dnl   done
  dnl fi
  dnl
  dnl if test -z "$DOTARRAY_DIR"; then
  dnl   AC_MSG_RESULT([not found])
  dnl   AC_MSG_ERROR([Please reinstall the dotarray distribution])
  dnl fi

  dnl # --with-dotarray -> add include path
  dnl PHP_ADD_INCLUDE($DOTARRAY_DIR/include)

  dnl # --with-dotarray -> check for lib and symbol presence
  dnl LIBNAME=dotarray # you may want to change this
  dnl LIBSYMBOL=dotarray # you most likely want to change this 

  dnl PHP_CHECK_LIBRARY($LIBNAME,$LIBSYMBOL,
  dnl [
  dnl   PHP_ADD_LIBRARY_WITH_PATH($LIBNAME, $DOTARRAY_DIR/lib, DOTARRAY_SHARED_LIBADD)
  dnl   AC_DEFINE(HAVE_DOTARRAYLIB,1,[ ])
  dnl ],[
  dnl   AC_MSG_ERROR([wrong dotarray lib version or lib not found])
  dnl ],[
  dnl   -L$DOTARRAY_DIR/lib -lm
  dnl ])
  dnl
  dnl PHP_SUBST(DOTARRAY_SHARED_LIBADD)

  PHP_NEW_EXTENSION(dotarray, dotarray.c, $ext_shared)
fi
