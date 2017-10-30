all: server client client_interest clean
	
server: server.cpp
	gcc -pthread -o server server.cpp

client: client.cpp
	gcc -o client client.cpp

client_interest: client_interest.cpp
	gcc -o client_interest client_interest.cpp

clean:
	rm -rf *o










