PHP_ARG_ENABLE(mp3info, mp3info,[ --enable-mp3info Enable mp3info support])
if test "$PHP_MP3INFO" = "yes"; then
  AC_DEFINE(HAVE_MP3INFO, 1, [mp3info support])
  PHP_NEW_EXTENSION(mp3info, php_mp3info.c mp3info.c mp3tech.c, $ext_shared)
fi
