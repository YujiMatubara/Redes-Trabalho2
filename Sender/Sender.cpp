#include "Sender.hpp"
#include "../Global.hpp"

Sender::Sender(){}
Sender::~Sender(){}

// Converts a String input into a string of 0 and 1 (it will work as a binary message)
std::string Sender::stringToBinary(std::string str) {
    std::string binRepresentation = "";
    std::string finalBinary = "";

    for (int i = 0; i < (int)str.length(); i++) {
        binRepresentation = "";

        // convert each char to ASCII value
        int asciiVal = int(str[i]);

        // Convert ASCII value to binary
        while (asciiVal > 0) {
            (asciiVal % 2) ? binRepresentation.push_back('1') : binRepresentation.push_back('0');
            asciiVal /= 2;
        }

        while ((int)binRepresentation.length() < 8) // each char must have a 8-bit representation
            binRepresentation.push_back('0');
        
        // Reverse the string for the next steps
        std::reverse(binRepresentation.begin(), binRepresentation.end());

        std::cout << "Done! Converted until '" << str[i] << "': " << binRepresentation << std::endl;

        finalBinary += binRepresentation;
    }
    std::cout << "Done! Converted evertything: " << finalBinary << std::endl;
    return finalBinary;
}

// Read the input, transform into a 0 and 1 string and so convert it to a bool vector
void Sender::applicationLayer() {
    std::string message;

    std::cout << "Digite uma mensagem:\n>> ";
    std::cin >> message;

    // Simulating converting from string to binary
    std::string binMsg = stringToBinary(message);

    // Transform the string into a bool vector
    for (int i = 0; i < (int)binMsg.length(); i++)
        inputBits.push_back( (binMsg[i] == '1') ? 1 : 0 );
    
    std::cout << "Input bits:\n";
    for (int i = 0; i < (int)inputBits.size(); i++)
        std::cout << inputBits[i];
    std::cout << std::endl;

    return;
}

// CRC encoding for verification
void Sender::CRC_32() {
    int crcSize = CRC_DIVISOR.size();

    if (crcSize == 0) {
        std::cerr << "[ERRO] Não é possível usar CRC com divisor de tamanho 0!\n";
        return;
    }

    // Aux vectors to use during XOR operations
    std::vector<bool> aux;
    
    // Append inputBits vector to aux vector
    aux.insert(aux.end(), inputBits.begin(), inputBits.end());
    
    // Insert zeroes to end of bit vector
    for (int i = 0; i < crcSize-1; i++)
        aux.push_back(0);
    

    // Could use a more efficient algorithm...
    for (int i = 0; i <= (int)aux.size() - crcSize; i++) { // Begins with the crcSize most significant bits from the message
        int crcIndex = 0;
        // std::cout << "\nAux vector before:\n";

        // for (int i = 0; i < (int)aux.size(); i++)
        //     std::cout << aux[i];
        // std::cout << std::endl;
        
        if (aux[i] == 0) { // if most significant bit of the last "remainder" is 0, XOR with 00000...000
            for (int j = i; j < i+crcSize; j++) { // makes XOR division bit by bit for crcSize bits
                // printf("j = %d: %d ^ 0\n", j, (aux[j]) ? 1 : 0);
                aux[j] = aux[j] ^ 0;
            }
        }
        else { // XOR with CRC divisor
            for (int j = i; j < i+crcSize; j++) { // makes XOR division bit by bit for crcSize bits
                // printf("j = %d: %d ^ %d\n", j, (aux[j]) ? 1 : 0, (CRC_DIVISOR[crcIndex]) ? 1 : 0 ); 
                aux[j] = aux[j] ^ CRC_DIVISOR[crcIndex];
                crcIndex++;
            }
        }
        // std::cout << std::endl;

        // std::cout << "Aux vector after:\n";

        // for (int i = 0; i < (int)aux.size(); i++)
        //     std::cout << aux[i];
        // std::cout << std::endl;
    }

    std::vector<bool> bitsCRC(crcSize-1);

    std::cout << "Aux vector after CRC algo.:\n";

    for (int i = 0; i < (int)aux.size(); i++)
        std::cout << aux[i];
    std::cout << std::endl;

    // crcSize-1 least significant bits are the ones we need to append to the message
    for (int i = 0; i < crcSize-1; i++) {
        bitsCRC[i] = aux[inputBits.size() + i];
    }
    
    std::cout << "CRC bits defined:\n";

    for (int i = 0; i < (int)bitsCRC.size(); i++)
        std::cout << bitsCRC[i];
    std::cout << std::endl;

    std::vector<bool> aux2 = inputBits;
    aux2.insert(aux2.end(), bitsCRC.begin(), bitsCRC.end());
    inputBits = aux2;

    std::cout << "Bit sequence after CRC applied:\n";

    for (int i = 0; i < (int)inputBits.size(); i++)
        std::cout << inputBits[i];
    std::cout << std::endl;
    
    return;
}


// Encoding for even and odd parity, set with the parameter
void Sender::bitParityEncoding(bool evenBitParity = true){
    int bitSum = 0;
    std::vector<bool> auxBits;
    bool lastBool;
    for(int i = 0; i < (int)inputBits.size();i++){
        lastBool = inputBits.at(i);
        auxBits.push_back(lastBool);
        bitSum += lastBool ? 1 : 0;
        if((i+1)%PARITY_RANGE == 0){
            auxBits.push_back(bitSum%2 == evenBitParity);
            bitSum = 0;
        }
    }
    if(inputBits.size()%PARITY_RANGE != 0) auxBits.push_back(bitSum%2 == evenBitParity);
    inputBits = auxBits;
    return;
}

// Run the link layer for the sender
std::vector<bool> Sender::linkLayer(int chosenErrorDetecAlg) {
    // Choosing error detection algorithm
    switch (chosenErrorDetecAlg)
    {
        case 0:
            //CRC_32();
        break;
        case 1: // even bit parity
            bitParityEncoding();
        break;
        case 2: // odd bit parity
            bitParityEncoding(false);
        break;
        default:
            std::cerr << "[ATENÇÃO] Método de correção de erro inválido. Escolha entre 0 e 2!\n";
    }

    std::cout << "Texto com encoding e sem frame\n";
    for (int i = 0; i < (int)inputBits.size(); i++)
        std::cout << inputBits[i];
    std::cout << "\n";

    // Framing: inserting frameFlag to beginning and end of frame (variable size)
    std::vector<bool> finalFrame; // frame we'll send to physical layer

    finalFrame.insert(finalFrame.end(), frameFlag.begin(), frameFlag.end());
    finalFrame.insert(finalFrame.end(), inputBits.begin(), inputBits.end());
    finalFrame.insert(finalFrame.end(), frameFlag.begin(), frameFlag.end());

    // returning to send to "pyshical layer" (communicationPath)
    return finalFrame;
}