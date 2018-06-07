OS := $(shell uname)

ifeq ($(OS), Linux)

PACKAGE = chat.zip
SOURCES = srv.cpp \
		  cln.cpp \
		  lib/blocking_queue.h lib/blocking_queue.cpp \
		  lib/thread_pool.h lib/thread_pool.cpp \
		  lib/my_epoll.h lib/my_epoll.cpp
LIBS = lib/thread_pool.o lib/blocking_queue.o lib/my_epoll.o
OUT_LIBS = 
GENERATED = $(LIBS) $(OUT_LIBS) server client 

all: $(GENERATED)

runs: server client
	rm -rf temp.temp.temp
	rm -rf temp.temp.temp.temp
	./server 4444 & chat $$! > temp.temp.temp 
	sleep 1
	./client 127.0.0.1 4444 & chat $$! > temp.temp.temp.temp
	sleep 2
	#kill `cat temp.temp.temp.temp` 
	kill `cat temp.temp.temp` 
	rm -rf temp.temp.temp
	rm -rf temp.temp.temp.temp

server : $(LIBS) srv.cpp
	$(CXX) -std=c++17 srv.cpp $(LIBS) -lpthread -O3 -o server

client : cln.cpp 
	$(CXX) -std=c++17 cln.cpp -O3 -o client

my_chat: $(LIBS)
	$(CXX) -std=c++17 $(LIBS) -O0 -o my_chat

lib/thread_pool.o : lib/thread_pool.h lib/thread_pool.cpp lib/blocking_queue.o
	$(CXX) -std=c++17 lib/thread_pool.cpp -c -o lib/thread_pool.o

lib/blocking_queue.o : lib/blocking_queue.h lib/blocking_queue.cpp
	$(CXX) -std=c++17 lib/blocking_queue.cpp -c -o lib/blocking_queue.o

lib/my_epoll.o : lib/my_epoll.h lib/my_epoll.cpp
	$(CXX) -std=c++17 lib/my_epoll.cpp -c -o lib/my_epoll.o

san: my_chat
		valgrind --leak-check=full --track-origins=yes ./my_chat 

clean: 
		rm -rf my_chat $(PACKAGE) $(GENERATED) *.o 

pack: $(SOURCES)
		zip $(PACKAGE) -r $(SOURCES) Makefile 
		
else


PACKAGE = chat.zip
SOURCES = srv.cpp \
	  	  cln.cpp 
LIBS = 
OUT_LIBS = 
GENERATED = $(LIBS) $(OUT_LIBS) server client 

all: $(GENERATED)

runs: server client
	rm -rf temp.temp.temp
	rm -rf temp.temp.temp.temp
	./server 4444 & chat $$! > temp.temp.temp 
	sleep 1
	./client 127.0.0.1 4444 & chat $$! > temp.temp.temp.temp
	sleep 2
	#kill `cat temp.temp.temp.temp` 
	kill `cat temp.temp.temp` 
	rm -rf temp.temp.temp
	rm -rf temp.temp.temp.temp

server : $(LIBS) srv.cpp
	$(CXX) -std=c++11 srv.cpp $(LIBS) -lpthread -O3 -o server

client : cln.cpp 
	$(CXX) -std=c++11 cln.cpp -O3 -o client

my_chat: $(LIBS)
	$(CXX) -std=c++11 $(LIBS) -O0 -o my_chat

san: my_chat
		valgrind --leak-check=full --track-origins=yes ./my_chat 

clean: 
		rm -rf my_chat $(PACKAGE) $(GENERATED) *.o 

pack: $(SOURCES)
		zip $(PACKAGE) -r $(SOURCES) Makefile 

endif

