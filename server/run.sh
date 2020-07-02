gcc `pkg-config --cflags gtk+-3.0` -o server src/server.c src/mx_send_data.c `pkg-config --libs gtk+-3.0`
./server
