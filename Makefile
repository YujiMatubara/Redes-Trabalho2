make all: CommunicationPath.o Receiver.o Sender.o main.o
	g++ -Wall CommunicationPath.o Receiver.o Sender.o -o Simulation

main.o: main.cpp global.hpp
	g++ -c main.cpp

CommunicationPath.o: CommunicationPath/CommunicationPath.cpp CommunicationPath/CommunicationPath.hpp Global.hpp
	g++ -c CommunicationPath/CommunicationPath.cpp

Receiver.o: Receiver/Receiver.cpp Receiver/Receiver.hpp Receiver/socketServer.hpp Global.hpp
	g++ -c Receiver/Receiver.cpp

Sender.o: Sender/Sender.cpp Sender/Sender.hpp Global.hpp
	g++ -c Sender/Sender.cpp

clean:
	rm -rf *.o