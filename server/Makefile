#test
APP = uchat_server

CC = clang
CFLAGS = -g -std=c11 $(addprefix -W, all extra pedantic) #Add error
DFLAGS = -fsanitize=address -fsanitize=undefined
GLIB = `pkg-config --cflags --libs glib-2.0  --libs gio-2.0 --libs gtk+-3.0`
cJSON = -I ../json/inc ../json/src/*.c
SRCS = -I inc src/*.c src/mailing/*.c src/requests/*.c

ZLIB = -I ../zlib/include/ ../zlib/lib/libz.a
CURL = $(ZLIB) -lldap -I ../curl/include/ ../curl/lib/.libs/libcurl.a

all: curl_install zlib_install
		@$(CC) $(CFLAGS) $(DFLAGS) $(GLIB) $(cJSON) $(CURL) -lsqlite3 $(SRCS) -o  $(APP)

curl_install:
	make -C ../curl

curl_uninstall:
	make uninstall -C ../curl

zlib_install:
	make -C ../zlib
	make install prefix="." -C ../zlib

zlib_uninstall:
	make uninstall -C ../zlib
	make uninstall prefix="." -C ../zlib
