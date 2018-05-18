PACKAGE = find.zip
SOURCES = my_find.c lib/my_utility.c lib/my_list.c lib/my_utility.h lib/my_list.h lib/shelllib/my_utility.h lib/shelllib/my_utility.c   
GENERATED = my_find.o lib/my_utility.o lib/my_list.o lib/shelllib/my_utility.o 

all: my_find

run: my_find 
	./my_find . --name my_utility.h --exec "ls -l" 

my_find: my_find.o $(GENERATED)
	gcc $(GENERATED) -O0 -o my_find

my_find.o: my_find.c
	gcc -c my_find.c

shell_lib: lib/shelllib/my_utility.c
	gcc -c lib/shelllib/my_utility.c

lib: lib/my_utility.c lib/my_list.c
	$(MAKE) shell_lib
	gcc -c lib/my_list.c -o lib/my_list.o
	gcc -c lib/my_utility.c -o lib/my_utility.o

san: my_find
	valgrind --leak-check=full --track-origins=yes my_find . --name my_utility.h  

clean: 
	rm -rf *.o my_find $(PACKAGE)
	rm -rf lib/*.o

pack: $(SOURCES)
	zip $(PACKAGE) -r $(SOURCES) Makefile
