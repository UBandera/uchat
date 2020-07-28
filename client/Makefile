# NAME = uchat_client
#
# CC = clang
# CFLAGS = -g -std=c11 $(addprefix -W, all extra pedantic) #Add error
# DFLAGS = -fsanitize=address -fsanitize=undefined
# UNUSED = -Wno-unused-parameter -Wno-unused-variable
# GLIB = `pkg-config --cflags --libs glib-2.0 gio-2.0 --libs gtk+-3.0`
#
# SRCD = src
# INCD = inc
# OBJD = obj
#
# JSOND = ../json
# JSON_INC = $(JSOND)/$(INCD)
# JSON_SRC = $(JSOND)/$(SRCD)
#
# REQD = $(SRCD)/requests
# RESD = $(SRCD)/responses
# SCREENSD = $(SRCD)/screens $(SRCD)/screens/auth $(SRCD)/screens/main_window
#
# SRCSD = $(SRCD) $(REQD) $(RESD) $(SCREENSD) $(JSON_SRC)
#
# SRCS = $(foreach dir, $(SRCSD), $(wildcard $(dir)/*.c))
# OBJS = $(addprefix $(OBJD)/, $(notdir $(SRCS:%.c=%.o)))
#
# INCS = -I $(JSON_INC) -I $(INCD)
#
# all:
# 	@$(CC) $(CFLAGS) $(DFLAGS) $(GLIB) $(UNUSED) $(SRCS) $(INCS) -o $(NAME)
#
# # all: $(NAME)
#
# install: all clean
#
# $(NAME): $(OBJS)
# 	@$(CC) $(CFLAGS) $(DFLAGS) $^ -o $@
# 	@printf "\e[32;1mProject $@ created\e[0m\n"
#
# $(OBJD)/%.o: %.c $(INCS)
# 	@$(CC) $(CFLAGS) $(DFLAGS) $(GLIB) -c $< -o $@ $(INCS)
# 	@printf "\e[32mcompiled: \e[0m[$(<:$(SRCD)/%.c=%.c)]\n"
#
# $(OBJS): | $(OBJD)
#
# $(OBJD):
# 	@mkdir -p $@
#
# uninstall: clean
# 	@rm -rf $(NAME)
# 	@printf "\e[34;1mProject $(NAME) deleted \e[0m\n"
#
# clean:
# 	@rm -rf $(OBJD)
# 	@printf "\e[34;1mDeleted object files in $(NAME)\e[0m\n"
#
# reinstall: uninstall install
# .PHONY: all uninstall clean reinstall

# name
CLIENT = uchat

EMPTY =
SPACE = $(EMPTY) $(EMPTY)
VPATH = VPATH = $(subst $(SPACE), :, $(SRCD))

# client working directory
CLIENT_WD = client/

OBJD = obj
# INCD_LIBS = -I../libraries/include
# LIBD_LIBS = -L../libraries/lib
INCD_CLIENT = -Iinc/ -I../json/inc/
# UTILS_INC = -I../utils/inc
# UTILS_LIB = ../utils/libutils.a

GTK_LIBS = -rdynamic `pkg-config --cflags --libs gtk+-3.0`
GTK_INC = `pkg-config --cflags gtk+-3.0`

SRCD = src src/requests src/responses src/screens src/screens/auth \
	   src/screens/main_window ../json/src

INCD = inc
SRCS = $(foreach dir, $(SRCD), $(wildcard $(dir)/*.c))
OBJS = $(addprefix $(OBJD)/, $(notdir $(SRCS:%.c=%.o)))
INCS = $(foreach dir, $(INCD), $(wildcard $(dir)/*.h))

OBJTEST = $(OBJS)
#compilier
CC = clang

CFLAGS = -std=c11 $(addprefix -W, all extra error pedantic)
CPPFLAGS = -DMX_CLIENT='"$(CLIENT_WD)"' $(INCD_CLIENT) -D_GNU_SOURCE \
		   $(GTK_INC) $(INCD_LIBS) -I/usr/local/opt/openssl/include
LIBS = $(GTK_LIBS) $(LIBD_LIBS) -L/usr/local/opt/openssl/lib \
	   -lssl -lcrypto -lsqlite3

all: $(CLIENT)

$(CLIENT): $(OBJS)
	@$(CC) -fsanitize=address -g -o $@ $^ $(LIBS)
	@printf "\033[32;1m$@ created\033[0m\n"

$(OBJS): obj/%.o: %.c $(INCS) | $(OBJD)
	@$(CC) $(CPPFLAGS) $(CFLAGS) -c -o $@ $<
	@printf "\033[32mcompiled: \033[0m$(notdir $<)\n"

$(OBJD):
	@mkdir -p $(CLIENT)_wd
	@mkdir -p $@
	@printf "\033[32;1m$@ created\033[0m\n"

uninstall: clean
	@rm -rf $(CLIENT)
	@printf "\033[34;1mdeleted $(CLIENT)\033[0m\n"

clean:
	@rm -rf $(OBJD)
	@rm -rf $(CLIENT_WD)/files
	@printf "\033[34;1mdeleted $(OBJD)\033[0m\n"

reinstall: uninstall all

.PHONY: all uninstall clean reinstall
