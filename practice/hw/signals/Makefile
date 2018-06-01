PACKAGE = signals.zip
SOURCES = my_signals.cpp \
		  lib/reentrancy_writer.h lib/reentrancy_writer.cpp \
		  lib/my_signal.h lib/my_signal.cpp
LIBS = lib/reentrancy_writer.o lib/my_signal.o
OUT_LIBS = 
GENERATED = my_signals.o $(LIBS) $(OUT_LIBS)

all: my_signals

run: my_signals 
		./my_signals raise
		./my_signals null
		./my_signals str

example1: example1_need 
	@rm -rf file.temp
	./example1 & echo $$! > file.temp
	kill -2 `cat file.temp`
	kill -2 `cat file.temp`
	kill -2 `cat file.temp`
	kill -2 `cat file.temp`
	@rm -rf file.temp

example2: example2_need
	@rm -rf file.temp
	./example2 & echo $$! > file.temp
	kill -2 `cat file.temp`
	kill -2 `cat file.temp`
	kill -2 `cat file.temp`
	kill -2 `cat file.temp`
	@rm -rf file.temp

my_signals: $(GENERATED)
	$(CXX) -std=c++17 $(GENERATED) -O0 -o my_signals

example1_need: signals_example.cpp $(LIBS)
	$(CXX) -std=c++17 -c signals_example.cpp 
	$(CXX) -std=c++17 signals_example.o $(LIBS) -o example1

example2_need: signals_example2.cpp $(LIBS)
	$(CXX) -std=c++17 -c signals_example2.cpp 
	$(CXX) -std=c++17 signals_example2.o $(LIBS) -o example2

my_signals.o: my_signals.cpp
		$(CXX) -std=c++17 -fpermissive -c my_signals.cpp

lib/reentrancy_writer.o: lib/reentrancy_writer.h lib/reentrancy_writer.cpp 
	$(CXX) -std=c++17 -c lib/reentrancy_writer.cpp -o lib/reentrancy_writer.o

lib/my_signal.o: lib/my_signal.h lib/my_signal.cpp
	$(CXX) -std=c++17 -c lib/my_signal.cpp -o lib/my_signal.o

san: my_signals
		valgrind --leak-check=full --track-origins=yes ./my_signals file 1.0

clean: 
		rm -rf my_signals $(PACKAGE) $(GENERATED) *.o example1 example2	

pack: $(SOURCES)
		zip $(PACKAGE) -r $(SOURCES) Makefile signals_example.cpp signals_example2.cpp
		
