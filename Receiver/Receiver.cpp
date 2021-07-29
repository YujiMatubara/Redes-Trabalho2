#include "Receiver.hpp"

//constructor and destructor empty
Receiver::Receiver(){}
Receiver::~Receiver(){}

//read the message
void Receiver::applicationLayer(std::vector<bool> outputBits){
    this->outputBits = outputBits;

    std::cout << "Message received:\n";

    if (outputBits.size() <= 0)  //if the message is empty
        std::cerr << "[Erro] Não há mensagem.";
    else    //if the message is reached, shows it
        for(bool outputBit : outputBits){   //iterates in outputBits
            std::cout << (outputBit ? "1" : "0"); //if true print 1, else (false) print 0
        }
    std::cout << "\n";
    
    return;
}

// Decoding for even and odd parity, set with the parameter eventBitParity
void Receiver::bitParityDecoding(bool evenBitParity = true){
    // std::cout << "Message arrived at B!\n";  //if the message was reached

    std::vector<std::vector<bool>> errorGroups;
    int bitSum = 0,errors = 0;
    wrongBlocks.resize(0);  //indicates where the error occurs
    int addedParityRange = PARITY_RANGE + 1;    //parity defined in global
    bool block,currentBit;

    for(int i = 0; i < (int)outputBits.size() - 1; i++){    //iterates in message
        currentBit = outputBits.at(i);
        bitSum += currentBit ? 1 : 0;
        if((i+1)%addedParityRange == 0){    //if the i is the parity flag
            block = (bitSum%2 == evenBitParity);
            wrongBlocks.push_back(block);
            errors += block ? 1 : 0;
            bitSum = 0;
        }
    }
    bitSum += outputBits.at((int)outputBits.size() - 1) ? 1 : 0;

    /* Prints the blocks of bits, when the parity is correct the color is green, when is wrong
    the color is green. Parity bit is always white */
    wrongBlocks.push_back(bitSum%2 == evenBitParity); 
    errors += block ? 1 : 0;
    int curParityPos;
    std::string message = "";   //initializate a null string
    std::cout << "Os blocos com erros existentes estão em vermelho e os corretos em verde. Os bits de paridade estão em branco:\n";
    for(int i=0;i<(int)wrongBlocks.size();i++){
        message += (wrongBlocks.at(i) ? "\x1B[31m" : "\x1B[32m" );
        for(int bitPos = i*addedParityRange;bitPos<(i+1)*addedParityRange - 1; bitPos++){
            if(bitPos == outputBits.size() - 1) break;
            message += (outputBits.at(bitPos) ? "1" : "0");
        }
        curParityPos = (i + 1 == (int)wrongBlocks.size() ? outputBits.back() : outputBits.at((i+1)*addedParityRange - 1));
        message += "\033[0m";
        message += (outputBits.at(curParityPos) ? "1 " : "0 ");

    }
    std::cout << message << std::endl;
    std::vector<bool> auxVector;
    for(int i=1; i<=outputBits.size();i++){
        // Verify if have to insert the last block (without parity bit) into the decoded vector
        if(i%addedParityRange != 0) auxVector.push_back(outputBits.at(i-1));
    }
    
    // Update outputBits vector
    outputBits = auxVector;

    return;
}

// CRC decoding for verification, returns the real size of the message
int Receiver::CRC_32(std::vector<bool> outputBits) {
    int crcSize = CRC_DIVISOR.size();

    if (crcSize == 0) {
        std::cerr << "[ERROR] Can't use CRC when CRC divisor's size is 0!\n";
        return -1;
    }

    // Aux vector to capture the crc flag
    std::vector<bool> aux = outputBits;

    
    for (int i = 0; i <= (int)aux.size() - crcSize; i++) { // Begins with the crcSize most significant bits from the message
        int crcIndex = 0;
        
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
    }

    std::cout << "CRC-32's result:\n";
    for (int i = 0; i < (int)aux.size(); i++) // Print the CRC-32 division
        std::cout << aux[i];
    std::cout << std::endl;

    for (int i = (int)aux.size()-1; i > (int)aux.size()-crcSize; i--) {
        // printf("Checking index %d = ", i);
        // std::cout << aux[i] << std::endl;
        if (aux[i] != 0) {  //if the result is not 0
            std::cerr << "CRC-32 identified problem in the division. Bit at position " << i << " looks odd... Your message may contain errors.\n";
        }
    }

    std::cout << "Message (without CRC flag):\n"; //print the message without the crc flag
    for (int i = 0; i <= (int)aux.size() - crcSize; i++) {
        std::cout << outputBits[i];
    }
    std::cout << std::endl;

    return (int)aux.size() - crcSize + 1;
}

std::string Receiver::getColoredMessage(std::string sentMessage){
    std::string coloredMessage = "";
    for(int i=0;i<sentMessage.size();i++){
        if(sentMessage[i] == receivedMessage[i]){
            coloredMessage += receivedMessage[i];
        }
        else{
            coloredMessage += "\x1B[31m";
            coloredMessage += receivedMessage[i];
            coloredMessage += "\033[0m";
        }
    }

    return coloredMessage;
}

void Receiver::linkLayer(int chosenErrorDetecAlg) {
    // Framing: removing frameFlag to beginning and end of frame (variable size)
    // Removes flags from begining and end

    int msgSize = 0; // Used only in CRC function
    std::vector<bool>::const_iterator first = outputBits.begin();   //iterator that begins the subvector
    std::vector<bool>::const_iterator last = outputBits.begin() + 8;    //iterator that ends the subvector
    std::vector<bool> beginCheck(first, last);  //create the begin flag

    first = outputBits.end() - 8;   //iterator that begins the subvector
    last = outputBits.end();    //iterator that ends the subvector
    std::vector<bool> endCheck(first, last);    //create the end flag

    if (beginCheck.size() != endCheck.size()) { //when the flags are not equal in size, they are not equal in content
        std::cout << "Flags' sizes not equal.\n";
        return;
    }
    for (int i = 0; i < beginCheck.size(); i++) { // if the size are the same check if the content is the same
        if (beginCheck[i] != endCheck[i]) {
            std::cout << "Error when compating flags.\n";
            return;
        }
    }
    std::cout << "No problems with the framing bits.\n";

    outputBits.erase(outputBits.begin(), outputBits.begin() + 8);   //removing the beginning flag
    outputBits.erase(outputBits.end() - 8, outputBits.end());   //removing the ending flag
    // std::cout << "Removeu a flag, a sequencia agora eh:\n";

    // for (int i = 0; i < (int)outputBits.size(); i++)    //print the message without the flag
    //     std::cout << outputBits[i];
    // std::cout << "\n";
    
    // Choosing error detection algorithm
    std::cout << chosenErrorDetecAlg;
    switch (chosenErrorDetecAlg)
    {
        case 0: //crc-32
            msgSize = CRC_32(outputBits);
        break;
        case 1: // even bit parity
            bitParityDecoding();
        break;
        case 2: // odd bit parity
            bitParityDecoding(false);
        break;
        default:
            std::cerr << "[WARNING] Invalid method of correction. Choose between 0 and 2!\n";
    }

    std::cout << "\n";

    // msgSize is calculated with different ways between CRC and bit parity
    if(chosenErrorDetecAlg != 0)
        msgSize = outputBits.size();

    // Print the decoded message
    if(msgSize % PARITY_RANGE != 0) {
        std::cout << "[ERROR] Some bits are missing\n";
        return;
    }
    // std::cout << "Message size: " << msgSize/PARITY_RANGE << " chars!\n";
    
    // Convert blocks of 8 bits in chars, and print each one
    int binToInt = 0;
    int pot = pow(2, PARITY_RANGE-1);
    
    //transform the binary sequence into a char string
    receivedMessage = "";
    
    for(int i = 0; i < msgSize; i++) {    //iterates the message getted

        if(outputBits[i] == 1)
            binToInt += pot;

        if(pot == 1) {  //if it is a true bollean
            //std::string a = std::accumulate(vec.begin(), vec.end(), std::string(""));
            receivedMessage += std::string(1,(char)binToInt);
            pot = pow(2, PARITY_RANGE-1);
            binToInt = 0;
        }
        else
            pot /= 2;
    }
    std::cout << receivedMessage << "\n";

    // returning to receive to "application layer"
}