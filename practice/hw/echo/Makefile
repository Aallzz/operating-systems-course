PACKAGE = echo.zip
SOURCES = server.cpp \
		  lib/blocking_queue.h lib/blocking_queue.cpp \
		  lib/thread_pool.h thread_pool.cpp \
		  lib/my_epoll.h lib/my_epoll.cpp
LIBS = lib/thread_pool.o lib/blocking_queue.o lib/my_epoll.o
OUT_LIBS = 
GENERATED = $(LIBS) $(OUT_LIBS)

runs: server
	./server

server : $(GENERATED) server.cpp
	$(CXX) -std=c++17 server.cpp $(GENERATED) -lpthread -O3 -o server

my_echo: $(GENERATED)
	$(CXX) -std=c++17 $(GENERATED) -O0 -o my_echo

lib/thread_pool.o : lib/thread_pool.h lib/thread_pool.cpp lib/blocking_queue.o
	$(CXX) -std=c++17 lib/thread_pool.cpp -c -o lib/thread_pool.o

lib/blocking_queue.o : lib/blocking_queue.h lib/blocking_queue.cpp
	$(CXX) -std=c++17 lib/blocking_queue.cpp -c -o lib/blocking_queue.o

lib/my_epoll.o : lib/my_epoll.h lib/my_epoll.cpp
	$(CXX) -std=c++17 lib/my_epoll.cpp -c -o lib/my_epoll.o

test1: lib/thread_pool.o thread_pool_test.o
	./test1

thread_pool_test.o : $(LIBS)
	$(CXX) -std=c++17 thread_pool_test.cpp $(LIBS) -lpthread -O3 -o test1


san: my_echo
		valgrind --leak-check=full --track-origins=yes ./my_echo 

clean: 
		rm -rf my_echo $(PACKAGE) $(GENERATED) *.o 

pack: $(SOURCES)
		zip $(PACKAGE) -r $(SOURCES) Makefile 
		
