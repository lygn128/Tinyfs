main:main.o config.o sds.o server.o NimbleStore.o utils.o Event.o Connection.o Packet.o
	g++ -o main main.o config.o sds.o server.o NimbleStore.o utils.o Event.o Connection.o Packet.o
sds.o:sds.h sds.cpp
	g++ -g -c sds.cpp
server.o:server.h server.cpp
	g++ -g -c server.cpp
config.o:config.h config.cpp
	g++ -g -c config.cpp 
NimbleStore.o:NimbleStore.h NimbleStore.cpp
	g++ -g -c NimbleStore.cpp
utils.o:utils.h utils.cpp
	g++ -g -c utils.cpp
Event.o:Event.h Event.cpp
	g++ -g -c Event.cpp
Connection.o:Connection.h Connection.cpp
	g++ -g -c Connection.cpp
Packet.o:Packet.h Packet.cpp
	g++ -g -c Packet.cpp
main.o:main.cpp
	g++ -g -c main.cpp

clean:
	rm *.o

