PACKAGE = jit.zip
SOURCES = my_jit.cpp \
		  lib/jit_function.h \
		  lib/memory_manager.h lib/memory_manager.cpp \
		  lib/my_utility.h lib/my_utility.cpp \
		  lib/operation_code.h \
		  lib/shelllib/my_utility.h 
LIBS = lib/memory_manager.o lib/my_utility.o 
OUT_LIBS = lib/shelllib/my_utility.o
GENERATED = my_jit.o $(LIBS) $(OUT_LIBS)

all: my_jit

run: my_jit 
		./my_jit file 1.0 

my_jit: my_jit.o $(GENERATED)
		g++ -std=c++14  $(GENERATED) -O0 -o my_jit

my_jit.o: my_jit.cpp
		g++ -std=c++14 -c my_jit.cpp

shell_lib: lib/shelllib/my_utility.c
	gcc -c  lib/shelllib/my_utility.c

lib: $(OUT_LIBS)
	$(MAKE) shell_lib
	g++ -std=c++14 -c lib/memory_manager.cpp -o lib/memory_manager.o
	g++ -std=c++14 -c lib/my_utility.cpp -o lib/my_utility.o 

san: my_jit
		valgrind --leak-check=full --track-origins=yes ./my_jit file 1.0

clean: 
		rm -rf my_jit $(PACKAGE) $(GENERATED)	

pack: $(SOURCES)
		zip $(PACKAGE) -r $(SOURCES) Makefile opcodes.study small_tests.sh disnamsm.sh
		
