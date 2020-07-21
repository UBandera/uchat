NAME = uchat_client

CC = clang
CFLAGS = -g -std=c11 $(addprefix -W, all extra pedantic) #Add error
DFLAGS = -fsanitize=address -fsanitize=undefined
GLIB = `pkg-config --cflags --libs glib-2.0 gio-2.0 --libs gtk+-3.0` -rdynamic

SRCD = src
INCD = inc
OBJD = obj

JSOND = ../json
JSON_INC = $(JSOND)/$(INCD)
JSON_SRC = $(JSOND)/$(SRCD)

REQD = $(SRCD)/requests
RESD = $(SRCD)/responses
SCREENSD = $(SRCD)/screens $(SRCD)/screens/auth

SRCSD = $(SRCD) $(REQD) $(RESD) $(SCREENSD) $(JSON_SRC)

SRCS = $(foreach dir, $(SRCSD), $(wildcard $(dir)/*.c))
OBJS = $(addprefix $(OBJD)/, $(notdir $(SRCS:%.c=%.o)))

INCS = -I $(JSON_INC) -I $(INCD)

all:
	@$(CC) $(CFLAGS) $(DFLAGS) $(GLIB) $(SRCS) $(INCS) -o $(NAME)

# all: $(NAME)

install: all clean

$(NAME): $(OBJS)
	@$(CC) $(CFLAGS) $(DFLAGS) $^ -o $@
	@printf "\e[32;1mProject $@ created\e[0m\n"

$(OBJD)/%.o: %.c $(INCS)
	@$(CC) $(CFLAGS) $(DFLAGS) $(GLIB) -c $< -o $@ $(INCS)
	@printf "\e[32mcompiled: \e[0m[$(<:$(SRCD)/%.c=%.c)]\n"

$(OBJS): | $(OBJD)

$(OBJD):
	@mkdir -p $@

uninstall: clean
	@rm -rf $(NAME)
	@printf "\e[34;1mProject $(NAME) deleted \e[0m\n"

clean:
	@rm -rf $(OBJD)
	@printf "\e[34;1mDeleted object files in $(NAME)\e[0m\n"

reinstall: uninstall install
.PHONY: all uninstall clean reinstall
