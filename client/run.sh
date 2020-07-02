gcc `pkg-config --cflags gtk+-3.0` -o client src/client.c src/mx_send_data.c src/screens/test.c `pkg-config --libs gtk+-3.0`
./client
