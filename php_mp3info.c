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

zval *get_zval_id3( mp3info *mp3 )
{
	zval *id3;	

	if( !mp3->id3_isvalid ) 
	{
		return 0;
	}

	id3 = emalloc( sizeof(zval) );
	
	if( id3 == 0 ) 
	{
		return 0;
	}

	array_init(id3);
	add_assoc_string(id3, "title", mp3->id3.title, 1);
	add_assoc_string(id3, "artist", mp3->id3.artist, 1);
	add_assoc_string(id3, "album", mp3->id3.album, 1);
	add_assoc_string(id3, "year", mp3->id3.year, 1);
	add_assoc_string(id3, "comment", mp3->id3.comment, 1);
	
	return id3;
}

zval *get_zval_header( mp3info *mp3 )
{
	zval *header;

	if( !mp3->header_isvalid )
	{
		return 0;
	}

	header = emalloc( sizeof(zval) );
	if( header == 0 )
	{
		return 0;
	}

	array_init(header);
	
	add_assoc_long(header, "sync", mp3->header.sync);
	add_assoc_long(header, "version", mp3->header.version);
	add_assoc_long(header, "version", mp3->header.layer);
	add_assoc_long(header, "version", mp3->header.crc);
	add_assoc_long(header, "bitrate", mp3->header.bitrate);
	add_assoc_long(header, "freq", mp3->header.freq);
	add_assoc_long(header, "padding", mp3->header.padding);
	add_assoc_long(header, "extension", mp3->header.extension);
	add_assoc_long(header, "mode", mp3->header.mode);
	add_assoc_long(header, "mode_extension", mp3->header.mode_extension);
	add_assoc_long(header, "copyright", mp3->header.copyright);
	add_assoc_long(header, "original", mp3->header.original);
	add_assoc_long(header, "emphasis", mp3->header.emphasis);

	return header;

}

PHP_FUNCTION(mp3_get_info)
{
	char *filename;
	int filename_len;
	int want_id3=1, scantype=SCAN_QUICK, fullscan_vbr=1;
	FILE  *fp;
	mp3info mp3;
	zval *id3, *header;	

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
	add_assoc_long(return_value, "frames", mp3.frames);
	add_assoc_long(return_value, "badframes", mp3.badframes);
	
	id3 = get_zval_id3( &mp3 );
	if( id3 != 0 ) 
	{	
		add_assoc_zval(return_value, "id3", id3);
	}

	header = get_zval_header( &mp3 );
	if( header != 0 )
	{
		add_assoc_zval(return_value, "header", header);
	}
}

