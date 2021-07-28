#include "Sender/Sender.hpp"
#include "Receiver/Receiver.hpp"
#include "CommunicationPath/CommunicationPath.hpp"
#include "Global.hpp"

/* Simulates the whole interaction between sender and receiver sending a binary message with
some flags to verification */


int main(int argc, char const *argv[]) {
    Sender sender;
    Receiver receiver;
    int encodingType = -1;

    /* Read what verification method will be used in Sender and Receiver (they have to use
    the same algorithm to work properly) */
    std::cout << "Escolha o método de verificação de erros\n0-CRC32\n1-Bit de paridade par\n2-Bit de pariade impar\n";
    std::cin >> encodingType;

    // The Sender functions starts here
    sender.applicationLayer();
    std::vector<bool> senderFrame = sender.linkLayer(encodingType); // can be 0, 1 or 2

    std::cout << "Texto com encoding e frame para verificação de erros\n";
    for (int i = 0; i < (int)senderFrame.size(); i++)
        std::cout << senderFrame[i];
    std::cout << "\n";

    // Simulates the communication path
    CP c;
    c.Error(senderFrame);
    std::cout << "Texto com possíveis erros do transporte\n";
    for (int i = 0; i < (int)senderFrame.size(); i++)
        std::cout << senderFrame[i];
    std::cout << "\n";

    // The receiver functions starts here
    receiver.applicationLayer(senderFrame);
    receiver.linkLayer(encodingType);/*
    std::string sentMessage = sender.getMessage();
    std::string receivedMessage = sender.getColoredMessage(sentMessage);
    std::cout << "Mensagens:\n\tEnviada: " << sentMessage << "\n\tRecebida: " << receivedMessage << std::endl;
    */return 0;
}
