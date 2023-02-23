# makefile

all: dataserver client

reqchannel.o: reqchannel.h reqchannel.cpp
	g++ -g -Wall -O1 -std=c++11 -c reqchannel.cpp

SafeBuffer.o: SafeBuffer.h SafeBuffer.cpp
	g++ -g -Wall -O1 -std=c++11 -c SafeBuffer.cpp

dataserver: dataserver.cpp reqchannel.o 
	g++ -g -Wall -O1 -std=c++11 -o dataserver dataserver.cpp reqchannel.o -lpthread -lrt

client: client_MP6.cpp reqchannel.o SafeBuffer.o
	g++ -g -Wall -O1 -std=c++11 -o client client_MP6.cpp reqchannel.o SafeBuffer.o -lpthread -lrt

clean:
	rm -rf *.o fifo* dataserver client
