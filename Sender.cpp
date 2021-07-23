#include "Sender.hpp"

Sender::Sender(){}

std::string Sender::stringToBinary(std::string str) {
    for (int i = 0; i <= (int)str.length(); i++) {
        // convert each char to ASCII value
        int asciiVal = int(str[i]);

        // Convert ASCII value to binary
        std::string binRepresentation = "";
        while (asciiVal > 0) {
            (asciiVal % 2) ? binRepresentation.push_back('1') : binRepresentation.push_back('0');
            asciiVal /= 2;
        }

        while (binRepresentation.length() < 8) // each char must have a 8-bit representation
            binRepresentation.push_back('0');
        
        std::reverse(binRepresentation.begin(), binRepresentation.end());

        printf("Done! Converted %s to %s\n", str, binRepresentation);

        return binRepresentation;
    }
}

void Sender::applicationLayer() {
    std::string message;

    std::cout << "Digite uma mensagem:\n>> ";
    std::cin >> message;

    // Simulating converting from string to binary
    std::string binMsg = stringToBinary(message);

    for (int i = 0; i < (int)binMsg.length(); i++)
        inputBits.push_back((binMsg[0] == '1') ? 1 : 0);

    return;
}

void Sender::bitParityEncoding(bool evenBitParity = true){
    int bitSum = 0;
    std::vector<bool> auxBits;
    bool lastBool;
    int it = 0;
    for(int i = (int)inputBits.size() - 1; i >= 0; i--){
        it++;
        lastBool = inputBits.at(i);
        auxBits.push_back(lastBool);
        bitSum += lastBool ? 1 : 0;
        if(it%PARITY_RANGE == 0){
            inputBits.push_back(bitSum%2 == evenBitParity);
            bitSum = 0;
        }        
    }
    if(inputBits.size()%PARITY_RANGE != 0) inputBits.push_back(bitSum%2 == evenBitParity);
    std::reverse(auxBits.begin(),auxBits.end());
    inputBits = auxBits;
    return;
}

void Sender::CRC_32() {
    int crcSize = CRC_DIVISOR.size();

    if (crcSize == 0) {
        std::cerr << "[ERRO] Não é possível usar CRC com divisor de tamanho 0!\n";
        return;
    }

    // Aux vector to make XOR operations
    std::vector<bool> aux(0, crcSize-1);
    // Append inputBits vector to aux vector
    aux.insert(aux.end(), inputBits.begin(), inputBits.end());

    for (int i = aux.size()-1; i >= 0 + crcSize; i--) { // Begins with the crcSize most significant bits from the message
        for (int j = i; j >= crcSize; j--) { // makes XOR division bit by bit for crcSize bits
            aux[j] ^= CRC_DIVISOR[j];
        }
    }

    std::vector<bool> bitsCRC(crcSize-1);

    // crcSize-1 least significant bits are the ones we need to append to the message
    for (int i = 0; i < crcSize; i++)
        bitsCRC[i] = aux[i]; 
    
    std::cout << "CRC bits defined:\n\t" << bitsCRC << std::endl;

    std::vector<bool> aux2 = bitsCRC;
    aux.insert(aux2.end(), inputBits.begin(), inputBits.end());
    inputBits = aux2;

    std::cout << "Bit sequence after CRC applied:\n\t" << inputBits << std::endl;
    
    return;
}

std::vector<bool> Sender::linkLayer(int chosenErrorDetecAlg) {
    // Choosing error detection algorithm
    switch(chosenErrorDetecAlg):
        case 0:
            CRC_32();
            break;
        case 1: // even bit parity
            bitParityEncoding(true);
            break;
        case 2: // odd bit parity
            bitParityEncoding(false);
            break;
        default:
            std::cerr << "[ATENÇÃO] Método de correção de erro inválido. Escolha entre 0 e 2!\n";

    // Framing: inserting frameFlag to beginning and end of frame (variable size)
    std::vector<bool> finalFrame; // frame we'll send to physical layer

    finalFrame.insert(finalFrame.end(), frameFlag.begin(), frameFlag.end());
    finalFrame.insert(finalFrame.end(), inputBits.begin(), inputBits.end());
    finalFrame.insert(finalFrame.end(), frameFlag.begin(), frameFlag.end());

    // returning to send to "pyshical layer" (communicationPath)
    return finalFrame;
}