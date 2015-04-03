/*
  +----------------------------------------------------------------------+
  | PHP Version 5                                                        |
  +----------------------------------------------------------------------+
  | Copyright (c) 1997-2012 The PHP Group                                |
  +----------------------------------------------------------------------+
  | This source file is subject to version 3.01 of the PHP license,      |
  | that is bundled with this package in the file LICENSE, and is        |
  | available through the world-wide-web at the following url:           |
  | http://www.php.net/license/3_01.txt                                  |
  | If you did not receive a copy of the PHP license and are unable to   |
  | obtain it through the world-wide-web, please send a note to          |
  | license@php.net so we can mail you a copy immediately.               |
  +----------------------------------------------------------------------+
  | Author:                                                              |
  +----------------------------------------------------------------------+
*/

/* $Id: header 321634 2012-01-01 13:15:04Z felipe $ */

#ifndef PHP_DOTARRAY_H
#define PHP_DOTARRAY_H

extern zend_module_entry dotarray_module_entry;
#define phpext_dotarray_ptr &dotarray_module_entry

#ifdef PHP_WIN32
#	define PHP_DOTARRAY_API __declspec(dllexport)
#elif defined(__GNUC__) && __GNUC__ >= 4
#	define PHP_DOTARRAY_API __attribute__ ((visibility("default")))
#else
#	define PHP_DOTARRAY_API
#endif

/*显示哈希结构*/
#define  DOTARAAY_HASH_TABLE 1
/*显示元素的双链表*/
#define  DOTARRAY_DOUBLE_LIST (1<<1)
/*现在遍历到那个位置了*/
#define  DOTARRAY_CURRENT_POSITION (1<<2)



#ifdef ZTS
#include "TSRM.h"
#endif

PHP_MINIT_FUNCTION(dotarray);
PHP_MSHUTDOWN_FUNCTION(dotarray);
PHP_RINIT_FUNCTION(dotarray);
PHP_RSHUTDOWN_FUNCTION(dotarray);
PHP_MINFO_FUNCTION(dotarray);




#ifdef ZTS
#define DOTARRAY_G(v) TSRMG(dotarray_globals_id, zend_dotarray_globals *, v)
#else
#define DOTARRAY_G(v) (dotarray_globals.v)
#endif

#endif


/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
