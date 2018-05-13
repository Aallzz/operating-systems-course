GENERATED = my_grep.c 
PACKAGE = grep.zip 
ARGS = -r main *

all : my_grep.o
	gcc my_grep.o -Wall -o my_grep

my_grep.o : my_grep.c
	gcc -c my_grep.c 

run : my_grep
	./my_grep $(ARGS)

clean : 
	rm -rf my_grep *.o $(PACKAGE)

pack: $(GENERATED)
	zip $(PACKAGE) -r $(GENERATED) Makefile 
