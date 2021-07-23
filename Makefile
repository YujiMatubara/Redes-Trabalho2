make all: CommunicationPath.o Receiver.o Sender.o Main.o
	g++ -Wall CommunicationPath.o Receiver.o Sender.o Main.o -o Simulation

Main.o: Main.cpp Global.hpp
	g++ -c Main.cpp

CommunicationPath.o: CommunicationPath/CommunicationPath.cpp CommunicationPath/CommunicationPath.hpp Global.hpp
	g++ -c CommunicationPath/CommunicationPath.cpp

Receiver.o: Receiver/Receiver.cpp Receiver/Receiver.hpp Global.hpp
	g++ -c Receiver/Receiver.cpp

Sender.o: Sender/Sender.cpp Sender/Sender.hpp Global.hpp
	g++ -c Sender/Sender.cpp

clean:
	rm -rf *.o