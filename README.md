# Redes-Trabalho2
2nd project for SCC0142

_Segundo trabalho para a disciplina SCC0142_

# Group Members
Gabriel Monteiro Ferracioli     11219129 \
Lucas Yuji Matubara             10734432 \
Pedro Guerra Lourenço           11218747 \
Vinicius Eduardo de Araujo      11275193

# Running the project

There's a simple Makefile tested in Ubuntu (Debian-based systems with g++ probably will work). To make all files, run `make all`.

To clean the folder, run `make clean`

The executable that needs to be run is the `Simulation` ELF file. Just run `./Simulation`

# Explaining files

The `Sender.cpp` and `Sender.hpp` implement  the functionalities related to the person sending the message.

The `Receiver.cpp` and `Receiver.hpp` implement the functionalities related to the person receiving the message.

The Data Link Layer and a simple Application Layer are simulated here.

Also, there a `CommunicationPath.cpp` file that simulates a real physical layer, with bit lost and interference.