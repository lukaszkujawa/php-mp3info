#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#include "php.h"
#include "php_mp3info.h"

static function_entry mp3info_functions[] = {
    PHP_FE(mp3_get_info, NULL)
    {NULL, NULL, NULL}
};

zend_module_entry mp3info_module_entry = {
#if ZEND_MODULE_API_NO >= 20010901
    STANDARD_MODULE_HEADER,
#endif
    PHP_MP3INFO_EXTNAME,
    mp3info_functions,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
#if ZEND_MODULE_API_NO >= 20010901
    PHP_MP3INFO_VERSION,
#endif
    STANDARD_MODULE_PROPERTIES
};

#ifdef COMPILE_DL_MP3INFO
ZEND_GET_MODULE(mp3info)
#endif

PHP_FUNCTION(mp3_get_info)
{
	char *filename;
	int filename_len;
	int want_id3=1, scantype=SCAN_QUICK, fullscan_vbr=1;
	FILE  *fp;
	mp3info mp3;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &filename, &filename_len) == FAILURE) {
       		RETURN_NULL();
	}

	if ( !( fp=fopen(filename,"rb") ) ) {
		RETURN_NULL();
	}

	memset(&mp3,0,sizeof(mp3info));
	mp3.filename=filename;
	mp3.file=fp;

	get_mp3_info(&mp3,scantype,fullscan_vbr);

	array_init(return_value);
	add_assoc_long(return_value, "seconds", mp3.seconds);
	add_assoc_string(return_value, "title", mp3.id3.title, 1);
	add_assoc_string(return_value, "artist", mp3.id3.artist, 1);
	add_assoc_string(return_value, "album", mp3.id3.album, 1);
	add_assoc_string(return_value, "year", mp3.id3.year, 1);
	add_assoc_string(return_value, "comment", mp3.id3.comment, 1);
}



