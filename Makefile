CC=g++
CFLAGS=-c -Wall -std=c++11


all: web-cli-compile

mk/con_args.o: wget/con_args.cpp
	$(CC) $(CFLAGS) -c -o mk/con_args.o wget/con_args.cpp

mk/urls.o: wget/urls.cpp
	$(CC) $(CFLAGS) -c -o mk/urls.o wget/urls.cpp

mk/sockets.o: wget/sockets.cpp
	$(CC) $(CFLAGS) -c -o mk/sockets.o wget/sockets.cpp

mk/crawler.o: wget/crawler.cpp
	$(CC) $(CFLAGS) -c -o mk/crawler.o wget/crawler.cpp

mk/parse_html.o: wget/parse_html.cpp
	$(CC) $(CFLAGS) -c -o mk/parse_html.o wget/parse_html.cpp -lgumbo

mk/main.o: wget/main.cpp
	$(CC) $(CFLAGS) -c -o mk/main.o wget/main.cpp

web-cli-compile: mk/con_args.o mk/urls.o mk/sockets.o mk/crawler.o mk/parse_html.o mk/main.o 
	$(CC)  -std=c++11  -o web-cli  mk/con_args.o mk/urls.o mk/sockets.o mk/crawler.o mk/parse_html.o mk/main.o  -lgumbo

clean:
	rm -rf web-cli *.o
