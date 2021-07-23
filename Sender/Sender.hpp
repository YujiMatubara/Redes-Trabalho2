#pragma once
#include "../Global.hpp"

class Sender {
    private:
        std::vector<bool> inputBits;  //simulating a bitset vector
    public:
        Sender();
        ~Sender();
        void applicationLayer();
        std::string stringToBinary(std::string str);
        std::vector<bool> linkLayer(int chosenErrorDetecAlg);
        void bitParityEncoding(bool evenBitParity);
        //void CRC_32();
};