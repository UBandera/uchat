#test
APP = uchat_server

CC = clang
CFLAGS = -g -std=c11 $(addprefix -W, all extra pedantic) #Add error
DFLAGS = -fsanitize=address -fsanitize=undefined
GLIB = `pkg-config --cflags --libs glib-2.0  --libs gio-2.0 --libs gtk+-3.0`
cJSON = -I ../json/inc ../json/src/*.c
SRCS = -I inc src/*.c src/mailing/*.c src/requests/*.c

ZLIB = -I ../zlib/include/ ../zlib/lib/libz.a
OPENSSL = -I /usr/local/opt/openssl/include /usr/local/opt/openssl/lib/libcrypto.a /usr/local/opt/openssl/lib/libssl.a
NGHTTP = -I /usr/local/opt/nghttp2/include/ /usr/local/Cellar/nghttp2/1.41.0/lib/libnghttp2.a
RTMDUMP = -I /usr/local/Cellar/rtmpdump/2.4+20151223_1/include /usr/local/Cellar/rtmpdump/2.4+20151223_1/lib/librtmp.a
BROTLI = -I /usr/local/Cellar/brotli/1.0.7/include /usr/local/Cellar/brotli/1.0.7/lib/libbrotlicommon-static.a /usr/local/Cellar/brotli/1.0.7/lib/libbrotlidec-static.a /usr/local/Cellar/brotli/1.0.7/lib/libbrotlienc-static.a
CURL = $(ZLIB) $(OPENSSL) $(NGHTTP) $(RTMDUMP) $(BROTLI) -lldap -I ../curl/include/ ../curl/lib/.libs/libcurl.a


all:
		@$(CC) $(CFLAGS) $(DFLAGS) $(GLIB) $(cJSON) $(CURL) -lsqlite3 $(SRCS) -o  $(APP)

curl_install:
	../curl/configure --with-ssl
	make -C ../curl

curl_uninstall:
	make uninstall -C ../curl

zlib_install:
	make -C ../zlib
	make install prefix="." -C ../zlib

zlib_uninstall:
	make uninstall -C ../zlib
	make uninstall prefix="." -C ../zlib
