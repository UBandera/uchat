#test
APP = uchat_server

CC = clang
CFLAGS = -g -std=c11 $(addprefix -W, all extra pedantic) #Add error
DFLAGS = -fsanitize=address -fsanitize=undefined
GLIB = `pkg-config --cflags --libs glib-2.0  --libs gio-2.0 --libs gtk+-3.0 --libs json-glib-1.0`
cJSON = -I ../json/inc $(wildcard ../json/src/*.c)
SQLITE3 = -lsqlite3
INC = -I inc $(SQLITE3) $(cJSON) $(GLIB)

SRCS = $(wildcard src/*.c)
# OBJS = $(subst src/, , $(SRCS:.c=.o))

OBJD = obj

all: $(APP)

$(APP): $(SRCS)
	$(CC) $(DFLAGS) $(INC) $^ -o $@
	@printf "\033[32;1m$@ created\033[0m\n"
