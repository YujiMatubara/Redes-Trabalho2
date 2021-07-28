#include "CommunicationPath.hpp"

//constructor and destructor empty
CP::CP(){}
CP::~CP(){}

/* when the message is in the link layer, some errors ocurrs, this class simulates these
    erros changing some bits of the message with the rand() function */
void CP::Error(std::vector<bool>& inputBits)
{
    for (int i = 0; i < inputBits.size(); i++) {    //iterates in inputBits
        if (rand()%100 < ERROR_PROB){   //when the rand is inside [0, ERROR_PROB]
            inputBits[i] = !inputBits[i];   //change the boolean vector in i to the oposite value
        }
    }

    return;
}