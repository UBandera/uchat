#test
APP = uchat_server

CC = clang
CFLAGS = -g -std=c11 $(addprefix -W, all extra pedantic) #Add error
DFLAGS = -fsanitize=address -fsanitize=undefined
GLIB = `pkg-config --cflags --libs glib-2.0  --libs gio-2.0 --libs gtk+-3.0`
cJSON = -I ../json/inc ../json/src/*.c
SRCS = -I inc src/*.c src/mailing/*.c src/requests/*.c

OPENSSL = -I /usr/local/opt/openssl/include /usr/local/opt/openssl/lib/libcrypto.a /usr/local/opt/openssl/lib/libssl.a
CURL = $(OPENSSL) -lldap -I ../curl/include/ ../curl/lib/.libs/libcurl.a
CURL1 = $(OPENSSL) -lldap -I ../curl1/curl/include/ ../curl1/curl/lib/.libs/libcurl.a


all:
		@$(CC) $(CFLAGS) $(DFLAGS) $(GLIB) $(cJSON) $(CURL) -lsqlite3 $(SRCS) -o  $(APP)

$(APP): $(SRCS)
	$(CC) $(DFLAGS) $(INC) $^ -o $@
	@printf "\033[32;1m$@ created\033[0m\n"
	
archive:
		@$(CC) $(CFLAGS) $(DFLAGS) $(GLIB) $(cJSON) $(CURL1) -lsqlite3 $(SRCS) -o  $(APP)

curl_archive_install:
	make -C ../curl1/curl

curl_install:
	mv ../Makefile ../curl
	make -C ../curl

curl_uninstall:
	make uninstall -C ../curl
