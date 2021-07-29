#pragma once
#include "../Global.hpp"
#include <cmath>

class Receiver {
    private:
        std::vector<bool> outputBits;
        std::vector<bool> wrongBlocks;
        std::string receivedMessage;
    public:
    Receiver();
    ~Receiver();
    std::string getColoredMessage(std::string sentMessage);
    void decodingMessage(std::vector<bool> outputBits);
    void bitParityDecoding(bool evenBitParity);
    std::string linkLayer(int chosenErrorDetecAlg);
    void applicationLayer(std::string receivedMessage);
    int CRC_32(std::vector<bool> outputBits);
    
};