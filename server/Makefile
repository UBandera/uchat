
SERVER = uchat_server

OPENSSL = -I /usr/local/opt/openssl/include 
CURL1 = $(OPENSSL) -I ../curl1/curl/include/ 
CURL_L = -lldap ../curl1/curl/lib/.libs/libcurl.a /usr/local/opt/openssl/lib/libcrypto.a /usr/local/opt/openssl/lib/libssl.a

EMPTY =
SPACE = $(EMPTY) $(EMPTY)
VPATH = VPATH = $(subst $(SPACE), :, $(SRCD))

# client working directory
SERVER_WD = server/$(SERVER)_wd

OBJD = obj
GLIB = `pkg-config --cflags --libs glib-2.0  --libs gio-2.0 --libs gtk+-3.0`
GLIBO = `pkg-config --cflags glib-2.0 gio-2.0 gtk+-3.0`
cJSON_H = -I ../json/inc 

SRCD = src src/mailing src/requests ../json/src
INCS = inc
SRCS = $(foreach dir, $(SRCD), $(wildcard $(dir)/*.c))
OBJS = $(addprefix $(OBJD)/, $(notdir $(SRCS:%.c=%.o)))
DB = -lsqlite3

#compilier
CC = clang

# CFLAGS = -std=c11 $(addprefix -W, all extra error pedantic)
CFLAGS = -std=c11 $(addprefix -W, all extra pedantic)

all: $(SERVER)

$(SERVER): $(OBJS)
	@$(CC) -o $@ $^ $(CURL_L) $(GLIB) $(DB)
	@printf "\033[32;1m$@ created\033[0m\n"

$(OBJS): obj/%.o: %.c $(INCS) | $(OBJD)
	@$(CC) $(CFLAGS) -I $(INCS) $(GLIBO) $(cJSON_H) $(CURL1) -c $< -o $@
	@printf "\033[32mcompiled: \033[0m$(notdir $<)\n"

$(OBJD):
	@mkdir -p $(SERVER)_wd
	@mkdir -p $@
	@printf "\033[32;1m$@ created\033[0m\n"

uninstall: clean
	@rm -rf $(SERVER)
	@printf "\033[34;1mdeleted $(SERVER)\033[0m\n"

clean:
	@rm -rf $(OBJD)
	@rm -rf $(SERVER_WD)/database.db
	@printf "\033[34;1mdeleted $(OBJD)\033[0m\n"

reinstall: uninstall all

.PHONY: all uninstall clean reinstall
