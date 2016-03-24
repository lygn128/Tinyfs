main:main.o config.o sds.o server.o NimbleStore.o utils.o Event.o Connection.o
	g++ -o main main.o config.o sds.o server.o NimbleStore.o utils.o Event.o Connection.o
sds.o:sds.h sds.cpp
	g++ -c sds.cpp
server.o:server.h server.cpp
	g++ -c server.cpp
config.o:config.h config.cpp
	g++ -c config.cpp 
NimbleStore.o:NimbleStore.h NimbleStore.cpp
	g++ -c NimbleStore.cpp
utils.o:utils.h utils.cpp
	g++ -c utils.cpp
Event.o:Event.h Event.cpp
	g++ -c Event.cpp
Connection.o:Connection.h Connection.cpp
	g++ -c Connection.cpp
main.o:main.cpp
	g++ -c main.cpp

clean:
	rm *.o

