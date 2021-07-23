#include "Receiver.hpp"

Receiver::Receiver(){}

Receiver::~Receiver(){}

int Receiver::applicationLayer(std::string message, std::vector<bool> inputBits) {

    std::cout << "Mensagem recebida:\n";

    if (message == "")  //if the message is empty
        std::cerr << "[Erro] Não há mensagem.\n";
    else    //if the message is reached, shows it
        std::cin >> message;
    
    return 0;
}

//P11P001
void Receiver::bitParityDecoding(bool evenBitParity = true){
    int bitSum = 0;
    wrongBlocks.resize(0);
    int it = 0,addedParityRange = PARITY_RANGE + 1;
    for(int i = (int)outputBits.size() - 1; i > 0; i--){
        it ++;
        bitSum += outputBits.at(i) ? 1 : 0;
        if(it%addedParityRange == 0){
            wrongBlocks.push_back(bitSum%2 != evenBitParity); 
            bitSum = 0;
        }
    }
    bitSum += outputBits.at(0) ? 1 : 0;
    wrongBlocks.push_back(bitSum%2 != evenBitParity); 

    std::reverse(wrongBlocks.begin(),wrongBlocks.end());
    
    return;
}

/*
    OBS: CONFERIR AS FLAGS NO REMOVE
*/
std::vector<bool> Receiver::linkLayer(int chosenErrorDetecAlg, std::vector<bool> finalFrame) {
    // Framing: removing frameFlag to beginning and end of frame (variable size)
    //int flag1 = finalFrame.remove(finalFrame.end(), frameFlag.begin(), frameFlag.end());
    //int flag2 = finalFrame.remove(finalFrame.end(), outputBits.begin(), outputBits.end());
    //int flag3 = finalFrame.remove(finalFrame.end(), frameFlag.begin(), frameFlag.end());

    // Choosing error detection algorithm
    switch (chosenErrorDetecAlg)
    {
        case 0:
            //CRC_32(); // A ATUALIZAR
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
        
    // returning to receive to "application layer"
    return finalFrame;
}