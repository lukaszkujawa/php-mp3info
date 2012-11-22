#include <stdio.h>
#include <stdlib.h>
#include "mp3tech.h"

#ifndef PHP_MP3INFO_H
#define PHP_MP3INFO_H 1
#define PHP_MP3INFO_VERSION "1.0"
#define PHP_MP3INFO_EXTNAME "mp3info"

PHP_FUNCTION(mp3_get_info);

extern zend_module_entry mp3info_module_entry;
#define phpext_mp3info_ptr &mp3info_module_entry

#endif


