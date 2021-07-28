#include "Receiver.hpp"

//constructor and destructor empty
Receiver::Receiver(){}
Receiver::~Receiver(){}

//read the message
void Receiver::applicationLayer(std::vector<bool> outputBits){
    this->outputBits = outputBits;

    std::cout << "Mensagem recebida:\n";

    if (outputBits.size() <= 0)  //if the message is empty
        std::cerr << "[Erro] Não há mensagem.";
    else    //if the message is reached, shows it
        for(bool outputBit : outputBits){   //iterates in outputBits
            std::cout << (outputBit ? "1" : "0");   //if true print 1, else (false) print 0
        }
    std::cout << "\n";
    
    return;
}

//P11P001
//
void Receiver::bitParityDecoding(bool evenBitParity = true){
    std::cout << "Chegou certo\n";
    std::vector<std::vector<bool>> errorGroups;  
    int bitSum = 0,errors = 0;
    wrongBlocks.resize(0);
    int addedParityRange = PARITY_RANGE + 1;
    bool block,currentBit;

    for(int i = 0; i < (int)outputBits.size() - 1; i++){
        currentBit = outputBits.at(i);
        bitSum += currentBit ? 1 : 0;
        if((i+1)%addedParityRange == 0){
            block = (bitSum%2 == evenBitParity);
            wrongBlocks.push_back(block);
            errors += block ? 1 : 0;
            bitSum = 0;
        }
    }
    bitSum += outputBits.at((int)outputBits.size() - 1) ? 1 : 0;
    wrongBlocks.push_back(bitSum%2 == evenBitParity); 
    errors += block ? 1 : 0;
    int curParityPos;
    std::string message = "";
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
        if(i%addedParityRange != 0) auxVector.push_back(outputBits.at(i-1));
    }
    outputBits = auxVector;

    return;
}

void Receiver::linkLayer(int chosenErrorDetecAlg) {
    // Framing: removing frameFlag to beginning and end of frame (variable size)
    // Removes flags from begining and end
    
    std::cout << "Removeu a flag, a sequencia agora eh\n";

    //std::vector<bool> begincheck = outputBits.substr();
    //std::vector<bool> endcheck;

    outputBits.erase(outputBits.begin(), outputBits.begin() + 8);
    outputBits.erase(outputBits.end() - 8, outputBits.end());
    for (int i = 0; i < (int)outputBits.size(); i++)
        std::cout << outputBits[i];
    std::cout << "\n";
    
    // Choosing error detection algorithm
    switch (chosenErrorDetecAlg)
    {
        case 0:
            //CRC_32();
        break;
        case 1: // even bit parity
            bitParityDecoding();
        break;
        case 2: // odd bit parity
            bitParityDecoding(false);
        break;
        default:
            std::cerr << "[ATENÇÃO] Método de correção de erro inválido. Escolha entre 0 e 2!\n";
    }

    std::cout << "\n";

    // Print the decoded message
    if(outputBits.size()%8 != 0) {
        std::cout << "ERROR: some bits are missing\n";
        return;
    }
    std::cout << "Message size: " << outputBits.size()/8 << " chars!\n";
    
    // Convert blocks of 8 bits in chars, and print each one
    int binToInt = 0;
    int pot = 128;
    
    for(int i = 0; i < outputBits.size(); i++) {

        if(outputBits[i] == true)
            binToInt += pot;

        if(pot == 1) {
            printf("%c", binToInt);
            pot = 128;
            binToInt = 0;
        }
        else
            pot /= 2;
    }
    std::cout << "\n";
    

    // returning to receive to "application layer"
}