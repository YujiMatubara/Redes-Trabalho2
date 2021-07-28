#pragma once
#include "../Global.hpp"

class Receiver {
    private:
        std::vector<bool> outputBits;
        std::vector<bool> wrongBlocks;
    public:
    Receiver();
    ~Receiver();
    void applicationLayer(std::vector<bool> outputBits);
    void bitParityDecoding(bool evenBitParity);
    void linkLayer(int chosenErrorDetecAlg);
    
};