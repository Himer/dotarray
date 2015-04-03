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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "php_ini.h"
#include "ext/standard/info.h"
#include "php_dotarray.h"
#include <ext/standard/php_smart_str.h>

/**
 * 得到元素的key
 */
static char* get_bucket_key(Bucket* bucket_p, unsigned int *format_length_p) {

	char *buffer = NULL;

	/*string key*/
	if (bucket_p->nKeyLength) {
		*format_length_p =
				(bucket_p->nKeyLength - 1) > 100 ?
						99 : (bucket_p->nKeyLength - 1);

		buffer = estrndup(bucket_p->arKey, bucket_p->nKeyLength - 1);

		/*数字key*/
	} else {
		buffer = emalloc(100);
		memset(buffer, 0, sizeof(buffer));
		*format_length_p = sprintf(buffer, "%lu", bucket_p->h);
	}

	return buffer;

}
/**
 * 创建数据节点
 */
static void dotarray_create_bucket(smart_str* buf, Bucket* bucket_p, long flag) {

	unsigned int format_length = 0;

	char * key = get_bucket_key(bucket_p, &format_length);

	smart_str_appendl(buf, "sk_item_", sizeof("sk_item_") - 1);

	smart_str_appendl(buf, key, format_length);

	smart_str_appendl(buf, "[label = \"<f0>key:",
			sizeof("[label = \"<f0>key:") - 1);

	smart_str_appendl(buf, key, format_length);

	smart_str_appendl(buf, "|<f1>pListNext*", sizeof("|<f1>pListNext*") - 1);

	smart_str_appendl(buf, "|<f2>pListLast*", sizeof("|<f2>pListLast*") - 1);

	smart_str_appendl(buf, "|<f3>pNext *", sizeof("|<f3>pNext *") - 1);

	smart_str_appendl(buf, "|<f4>pLast*", sizeof("|<f4>pLast*") - 1);

	smart_str_appendl(buf, "\"];\r\n", sizeof("\"];\r\n") - 1);

	if (flag & DOTARAAY_HASH_TABLE) {
		if (bucket_p->pNext) {

			smart_str_appendl(buf, "sk_item_", sizeof("sk_item_") - 1);

			smart_str_appendl(buf, key, format_length);

			smart_str_appendl(buf, ":f3:s -> ", sizeof(":f3:s -> ") - 1);

			smart_str_appendl(buf, "sk_item_", sizeof("sk_item_") - 1);

			unsigned int next_format_length = 0;

			char *nextName = get_bucket_key(bucket_p->pNext,
					&next_format_length);

			smart_str_appendl(buf, nextName, next_format_length);

			efree(nextName);

			smart_str_appendl(buf, ":f3;\r\n", sizeof(":f3;\r\n") - 1);

		}

		if (bucket_p->pLast) {

			smart_str_appendl(buf, "sk_item_", sizeof("sk_item_") - 1);

			smart_str_appendl(buf, key, format_length);

			smart_str_appendl(buf, ":f4:s -> ", sizeof(":f4:s -> ") - 1);

			smart_str_appendl(buf, "sk_item_", sizeof("sk_item_") - 1);

			unsigned int last_format_length = 0;

			char *lastName = get_bucket_key(bucket_p->pLast,
					&last_format_length);

			smart_str_appendl(buf, lastName, last_format_length);

			efree(lastName);
			smart_str_appendl(buf, ":f4 [style=dotted];\r\n", sizeof(":f4 [style=dotted];\r\n") - 1);
		}

	}

	if (flag & DOTARRAY_DOUBLE_LIST) {
		smart_str_appendl(buf, "edge [color=red];\r\n",
				sizeof("edge [color=red];\r\n") - 1);

		if (bucket_p->pListNext) {

			smart_str_appendl(buf, "sk_item_", sizeof("sk_item_") - 1);

			smart_str_appendl(buf, key, format_length);

			smart_str_appendl(buf, ":f1:s -> ", sizeof(":f1:s -> ") - 1);

			smart_str_appendl(buf, "sk_item_", sizeof("sk_item_") - 1);

			unsigned int next_format_length = 0;

			char *nextName = get_bucket_key(bucket_p->pListNext,
					&next_format_length);

			smart_str_appendl(buf, nextName, next_format_length);

			efree(nextName);

			smart_str_appendl(buf, ":f1;\r\n", sizeof(":f1;\r\n") - 1);

		}

		if (bucket_p->pListLast) {

			smart_str_appendl(buf, "sk_item_", sizeof("sk_item_") - 1);

			smart_str_appendl(buf, key, format_length);

			smart_str_appendl(buf, ":f2:s -> ", sizeof(":f2:s -> ") - 1);

			smart_str_appendl(buf, "sk_item_", sizeof("sk_item_") - 1);

			unsigned int last_format_length = 0;

			char *lastName = get_bucket_key(bucket_p->pListLast,
					&last_format_length);

			smart_str_appendl(buf, lastName, last_format_length);

			efree(lastName);
			smart_str_appendl(buf, ":f2 [style=dotted];\r\n", sizeof(":f2 [style=dotted];\r\n") - 1);

		}

		smart_str_appendl(buf, "edge [color=black];\r\n",
				sizeof("edge [color=black];\r\n") - 1);
	}

	efree(key);

}

ZEND_FUNCTION(dotarray) {

	zval *param_array_p;

	long flag = 1;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "a|l", &param_array_p,
			&flag) == FAILURE) {
		php_error_docref(NULL TSRMLS_CC, E_WARNING, "param error.");

		RETURN_FALSE

	}

	if (Z_TYPE_P(param_array_p) != IS_ARRAY) {

		php_error_docref(NULL TSRMLS_CC, E_WARNING, "param should be array.");

		RETURN_FALSE

	}

	smart_str buf = { 0 };

	HashTable *arr_hash = Z_ARRVAL_P(param_array_p);

	long cnt = zend_hash_num_elements(arr_hash);

	smart_str_appendl(&buf,
			"digraph html {label = \"Structure of array\";\r\n node[shape = record];\r\n ",
			sizeof("digraph html {label = \"Binary search tree\";\r\n node[shape = record];\r\n")
					- 1);

	smart_str_appendl(&buf, "sk_array[label = \"<f0> nTableSize:",
			sizeof("sk_array[label = \"<f0> nTableSize:") - 1);
	smart_str_append_long(&buf, arr_hash->nTableSize);

	smart_str_appendl(&buf, "|<f1>nTableMask:", sizeof("|<f1>nTableMask:") - 1);
	smart_str_append_long(&buf, arr_hash->nTableMask);

	smart_str_appendl(&buf, "|<f2>nNumOfElements:",
			sizeof("|<f2>nNumOfElements:") - 1);
	smart_str_append_long(&buf, arr_hash->nNumOfElements);

	smart_str_appendl(&buf, "|<f3>nNextFreeElement:",
			sizeof("|<f3>nNextFreeElement:") - 1);
	smart_str_append_long(&buf, arr_hash->nNextFreeElement);

	smart_str_appendl(&buf, "|<f4>arBuckets**", sizeof("|<f4>arBuckets**") - 1);

	smart_str_appendl(&buf, "|<f5>pInternalPointer*",
			sizeof("|<f5>pInternalPointer*") - 1);

	smart_str_appendl(&buf, "|<f6>pListHead*", sizeof("|<f6>pListHead*") - 1);

	smart_str_appendl(&buf, "|<f7>pListTail*", sizeof("|<f7>pListTail*") - 1);

	smart_str_appendl(&buf, "\"];\r\n", sizeof("\"];\r\n") - 1);

	if (flag & DOTARAAY_HASH_TABLE) {

		/*Bucket的dot*/
		if (arr_hash->nTableSize) {

			unsigned int index = 0, format_length = 0;

			char buffer[100];

			smart_str_appendl(&buf, "sk_bucket[label = \"",
					sizeof("sk_bucket[label = \"") - 1);

			for (; index < arr_hash->nTableSize; index++) {

				if (index) {
					smart_str_appendc(&buf, '|');
				}

				format_length = sprintf(buffer, "<f%d> Bucket*:%d", index,
						index);

				smart_str_appendl(&buf, buffer, format_length);

			}
			smart_str_appendl(&buf, "\"];\r\n", sizeof("\"];\r\n") - 1);

			smart_str_appendl(&buf, "sk_array:f4:s -> sk_bucket:f0;\r\n",
					sizeof("sk_array:f4:s -> sk_bucket:f0;\r\n") - 1);
		}

	}

	/*item的dot*/
	if (arr_hash->nTableSize) {

		Bucket* bucket_p = NULL;

		unsigned int index = 0, format_length = 0;

		char buffer[500];

		for (; index < arr_hash->nTableSize; index++) {

			memset(buffer, 0, sizeof(buffer));

			bucket_p = arr_hash->arBuckets[index];

			if (flag & DOTARAAY_HASH_TABLE) {

				if (NULL != bucket_p) {

					format_length = sprintf(buffer,
							"sk_bucket:f%d:s -> sk_item_", index);

					smart_str_appendl(&buf, buffer, format_length);

					char *item_key = get_bucket_key(bucket_p, &format_length);

					smart_str_appendl(&buf, item_key, format_length);

					efree(item_key);

					smart_str_appendl(&buf, ":f0;\r\n", sizeof(":f0;\r\n") - 1);

				}

			}

			while (NULL != bucket_p) {

				dotarray_create_bucket(&buf, bucket_p, flag);

				bucket_p = bucket_p->pNext;

			}

		}

	}

	if (flag & DOTARRAY_DOUBLE_LIST) {

		smart_str_appendl(&buf, "edge [color=red];\r\n",
				sizeof("edge [color=red];\r\n") - 1);
		/*全部数据组成的双链表的头节点*/
		if (arr_hash->pListHead) {

			unsigned int format_length = 0;

			smart_str_appendl(&buf, "sk_array:f6:s -> sk_item_",
					sizeof("sk_array:f6:s -> sk_item_") - 1);
			char *headName = get_bucket_key(arr_hash->pListHead,
					&format_length);

			smart_str_appendl(&buf, headName, format_length);

			efree(headName);

			smart_str_appendl(&buf, ":f1;\r\n", sizeof(":f1;\r\n") - 1);

		}

		/*全部数据组成的双链表的尾节点*/
		if (arr_hash->pListTail) {

			unsigned int format_length = 0;

			smart_str_appendl(&buf, "sk_array:f7:s -> sk_item_",
					sizeof("sk_array:f7:s -> sk_item_") - 1);
			char *tailName = get_bucket_key(arr_hash->pListTail,
					&format_length);

			smart_str_appendl(&buf, tailName, format_length);

			efree(tailName);

			smart_str_appendl(&buf, ":f2 [style=dotted];\r\n", sizeof(":f2 [style=dotted];\r\n") - 1);

		}

		smart_str_appendl(&buf, "edge [color=black];\r\n",
				sizeof("edge [color=black];\r\n") - 1);
	}

	if (flag & DOTARRAY_CURRENT_POSITION) {

		if (arr_hash->pInternalPointer) {

			smart_str_appendl(&buf, "edge [color=green];\r\n",
					sizeof("edge [color=green];\r\n") - 1);

			unsigned int format_length = 0;

			smart_str_appendl(&buf, "sk_array:f5:s -> sk_item_",
					sizeof("sk_array:f5:s -> sk_item_") - 1);
			char *posName = get_bucket_key(arr_hash->pInternalPointer,
					&format_length);

			smart_str_appendl(&buf, posName, format_length);

			efree(posName);

			smart_str_appendl(&buf, ":f0;\r\n", sizeof(":f0;\r\n") - 1);

			smart_str_appendl(&buf, "edge [color=black];\r\n",
					sizeof("edge [color=black];\r\n") - 1);

		}

	}

	smart_str_appendc(&buf, '}');

	smart_str_0(&buf);

	RETURN_STRINGL(buf.c, buf.len, 1);

	smart_str_free(&buf);
}

static int le_dotarray;

const zend_function_entry dotarray_functions[] = {
ZEND_FE(dotarray,NULL)
PHP_FE_END };

zend_module_entry dotarray_module_entry = {
#if ZEND_MODULE_API_NO >= 20010901
		STANDARD_MODULE_HEADER,
#endif
		"dotarray", dotarray_functions,
		PHP_MINIT(dotarray),
		PHP_MSHUTDOWN(dotarray),
		PHP_RINIT(dotarray),
		PHP_RSHUTDOWN(dotarray),
		PHP_MINFO(dotarray),
#if ZEND_MODULE_API_NO >= 20010901
		"0.1",
#endif
		STANDARD_MODULE_PROPERTIES };

#ifdef COMPILE_DL_DOTARRAY
ZEND_GET_MODULE(dotarray)
#endif

PHP_MINIT_FUNCTION(dotarray) {

	REGISTER_LONG_CONSTANT("DOTARAAY_HASH_TABLE", DOTARAAY_HASH_TABLE, CONST_CS | CONST_PERSISTENT);

	REGISTER_LONG_CONSTANT("DOTARRAY_DOUBLE_LIST", DOTARRAY_DOUBLE_LIST, CONST_CS | CONST_PERSISTENT);

	REGISTER_LONG_CONSTANT("DOTARRAY_CURRENT_POSITION", DOTARRAY_CURRENT_POSITION, CONST_CS | CONST_PERSISTENT);

	return SUCCESS;
}

PHP_MSHUTDOWN_FUNCTION(dotarray) {
	return SUCCESS;
}

PHP_RINIT_FUNCTION(dotarray) {
	return SUCCESS;
}

PHP_RSHUTDOWN_FUNCTION(dotarray) {
	return SUCCESS;
}

PHP_MINFO_FUNCTION(dotarray) {
	php_info_print_table_start();
	php_info_print_table_header(2, "dotarray support", "enabled");
	php_info_print_table_end();

}

