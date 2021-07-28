#pragma once
#include "../Global.hpp"
#include <time.h>

class CP {
    private:
        std::vector<bool> inputBits;  //simulating a bitset vector
    public:
        CP();
        ~CP();
        void Error(std::vector<bool>& inputBits);
};