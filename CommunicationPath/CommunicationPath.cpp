#include "CommunicationPath.hpp"

CP::CP(){}
CP::~CP(){}
void Error(std::vector<bool> inputBits)
{
    double errorpercent = ERROR_PROB;  //error probability
    for (int i = 0; i < inputBits.size() - 1; i++) {
        if (!rand()%100 > errorpercent)
            inputBits[i] = !inputBits[i];
    }
    return;
}