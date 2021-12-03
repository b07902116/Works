all:
	g++ sender.cpp -o sender `pkg-config --cflags --libs opencv`
	g++ receiver.cpp -o receiver `pkg-config --cflags --libs opencv`
	g++ agent.c -o agent

sender:
	g++ sender.cpp -o sender `pkg-config --cflags --libs opencv`

receiver:
	g++ receiver.cpp -o receiver `pkg-config --cflags --libs opencv`

agent:
	g++ agent.c -o agent

