#pragma once
#include "../Global.hpp"

class Receiver {
    private:
        std::vector<bool> outputBits;
        std::vector<bool> wrongBlocks;
    public:
    Receiver();
    ~Receiver();
    int applicationLayer(std::string message, std::vector<bool> inputBits);
    void bitParityDecoding(bool evenBitParity);
    std::vector<bool> linkLayer(int chosenErrorDetecAlg, std::vector<bool> finalFrame);
    
};