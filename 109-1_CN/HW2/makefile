all:
	g++ client.cpp -o client `pkg-config --cflags --libs opencv`
	g++ server.cpp -o server `pkg-config --cflags --libs opencv`

client: client.cpp
	g++ client.cpp -o client `pkg-config --cflags --libs opencv`

server: server.cpp
	g++ server.cpp -o server `pkg-config --cflags --libs opencv`
