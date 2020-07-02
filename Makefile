SERVER = uchat_server

CC = clang
CFLAGS = -g -std=c11 $(addprefix -W, all extra pedantic) #Add error
DFLAGS = -fsanitize=address -fsanitize=undefined 
GLIB = `pkg-config --cflags --libs glib-2.0 --libs gio-2.0`


all:
	# $(GLIB)
	$(CC) $(CFLAGS) $(DFLAGS) $(GLIB) *.c
