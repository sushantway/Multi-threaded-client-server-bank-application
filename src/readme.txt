There are mainly three .cpp files as follows:

1.server.cpp - server code
compile: gcc -pthread -o server server.cpp
run: ./server 8080

2.client.cpp
compile: gcc -o client client.cpp
run: ./client hostname 8080 transactions.txt 0.1

3.client_interest.cpp
compile: gcc -o client_interest client_interest.cpp
run: ./client_interest hostname 8080