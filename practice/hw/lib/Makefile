PACKAGE = lib.zip
SOURCES = my_lib.cpp \
		  lib/process_info.h lib/process_info.cpp \
		  lib/circle_pattern.h lib/circle_pattern.cpp \
		  lib/square_pattern.h lib/square_pattern.cpp \
		  lib/run_time_lib_function.h 
LIBS = lib/process_info.o lib/process_info.a 
DLIBS = lib/square_pattern.o lib/libsquare_pattern.so
RUNTIME_LIBS = lib/circle_pattern.o lib/libcircle_pattern.so
OUT_LIBS = 
GENERATED = my_lib.o $(LIBS) $(OUT_LIBS) $(RUNTIME_LIBS) $(DLIBS)

all: my_lib

run1: my_lib 
		./my_lib

run2: my_lib 
		./my_lib circle 20 

my_lib: my_lib.o $(GENERATED) $(DLIBS) $(RUNTIME_LIBS)
		g++ -std=c++14 my_lib.o -L./lib -lsquare_pattern -Wl,-rpath,./lib lib/process_info.a -ldl -O0 -o my_lib

lib: $(LIBS) $(DLIBS) $(RUNTIME_LIBS)

my_lib.o: my_lib.cpp
		g++ -std=c++14 -c my_lib.cpp

lib/process_info.a: lib/process_info.cpp lib/process_info.h 
	g++ -std=c++14 -c lib/process_info.cpp -o lib/process_info.o
	ar rcs lib/process_info.a lib/process_info.o
	ranlib lib/process_info.a 

lib/libcircle_pattern.so: lib/circle_pattern.cpp lib/circle_pattern.h
	g++ -std=c++14 -fPIC -c lib/circle_pattern.cpp -o lib/circle_pattern.o 
	g++ -std=c++14 -shared lib/circle_pattern.o -o lib/libcircle_pattern.so

lib/libsquare_pattern.so: lib/square_pattern.cpp lib/square_pattern.h
	g++ -std=c++14 -fPIC -c lib/square_pattern.cpp -o lib/square_pattern.o 
	g++ -std=c++14 -shared lib/square_pattern.o -o lib/libsquare_pattern.so

san1: my_lib
		valgrind --leak-check=full --track-origins=yes ./my_lib

san2: my_lib
		valgrind --leak-check=full --track-origins=yes ./my_lib circle 12

clean: 
		rm -rf my_lib $(PACKAGE) $(GENERATED) $(RUNTIME_LIBS)	

pack: $(SOURCES)
		zip $(PACKAGE) -r $(SOURCES) Makefile
		
