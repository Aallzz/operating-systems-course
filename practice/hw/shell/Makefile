PACKAGE = shell.zip
GENERATED = my_shell.c lib/my_utility.c lib/my_utility.h 

all: my_shell

run: my_shell 
	./my_shell

my_shell: my_shell.o lib/my_utility.o
	gcc my_shell.o lib/my_utility.o -O0 -o my_shell

my_shell.o: my_shell.c
	gcc -c my_shell.c

lib: lib/my_utility.c
	echo "ASD"
	gcc -c lib/my_utility.c -o lib/my_utility.o

san: my_shell
	valgrind --leak-check=full --track-origins=yes my_shell

clean: 
	rm -rf *.o my_shell $(PACKAGE)
	rm -rf lib/*.o

pack: $(GENERATED)
	zip $(PACKAGE) -r $(GENERATED) Makefile
