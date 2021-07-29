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

    std::cout << "==== SENDER ===\n";

    /* Read what verification method will be used in Sender and Receiver (they have to use
    the same algorithm to work properly) */
    std::cout << "Choose the erro validation method:\n0) CRC32\n1) Bit de paridade par\n2) Bit de pariade impar\n>> ";
    std::cin >> encodingType;

    // The Sender functions starts here
    sender.applicationLayer();
    std::vector<bool> senderFrame = sender.linkLayer(encodingType); // can be 0, 1 or 2

    // Simulates the communication path
    CP pyshicalLayer;
    pyshicalLayer.Error(senderFrame);

    std::cout << "Message transmitted from A (sender) to B (receiver):\n";
    for (int i = 0; i < (int)senderFrame.size(); i++)
        std::cout << senderFrame[i];
    std::cout << "\n\n";

    // The receiver functions starts here
    std::cout << "==== RECEIVER ===\n";
    receiver.applicationLayer(senderFrame);
    receiver.linkLayer(encodingType);
    std::string sentMessage = sender.getMessage();
    std::string receivedMessage = receiver.getColoredMessage(sentMessage);
    std::cout << "\nMensagens:\n\tEnviada: " << sentMessage << "\n\tRecebida: " << receivedMessage << std::endl;
    return 0;
}
