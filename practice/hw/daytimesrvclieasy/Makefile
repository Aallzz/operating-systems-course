PACKAGE=daytimesrvcli.zip
run1: all
	./client 192.6.38.100 

run2: all
	@rm -rf temp.temp.temp
	@rm -rf temp.temp.temp.temp
	./server & echo $$! > temp.temp.temp 
	@sleep 1
	./client 127.0.0.1 & echo $$! > temp.temp.temp.temp
	@sleep 2
	@kill `cat temp.temp.temp.temp`
	@kill `cat temp.temp.temp`
	@rm -rf temp.temp.temp
	@rm -rf temp.temp.temp.temp

all: client.c server.c
	gcc client.c -lpthread -o client
	gcc server.c -lpthread -o server

clean: 
	rm -rf *.o client server
	rm -rf $(PACKAGE)

pack: client.c server.c
	zip $(PACKAGE) -r client.c server.c Makefile	
