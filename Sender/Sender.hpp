#pragma once
#include "../Global.hpp"

class Sender {
    private:
        std::vector<bool> inputBits;  //simulating a bitset vector
        std::string sentMessage;
    public:
        Sender();
        ~Sender();
        std::string getMessage();
        void applicationLayer();
        std::string stringToBinary(std::string str);
        std::vector<bool> linkLayer(int chosenErrorDetecAlg);
        void bitParityEncoding(bool evenBitParity);
        void CRC_32();
};