

COMMON := core/Reactor.cc core/Socket.cc core/LogWrite.cc core/Proto.cc



CC := g++ -g -Wall -fpermissive -std=c++14

all:
	$(CC) Server.cc $(COMMON) -Icore -I. -o Server -lpthread
	$(CC) Client.cc $(COMMON) -Icore -I. -o Client -lpthread


clean: 
	rm -rf *.o core/*.o
