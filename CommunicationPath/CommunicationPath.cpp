#include "CommunicationPath.hpp"

//constructor and destructor empty
CP::CP(){}
CP::~CP(){}

/* when the message is in the link layer, some errors ocurrs, this class simulates these
    erros changing some bits of the message with the rand() function */
void CP::Error(std::vector<bool>& inputBits)
{
    // Creates the seed for pseudo-random numbers
    srand(time(0));
    for (int i = 0; i < inputBits.size(); i++) {    //iterates in inputBits
        int r = rand()%100;
        if (r < ERROR_PROB){   //when the rand is inside [0, ERROR_PROB]
            inputBits[i] = !inputBits[i];   //change the boolean vector in i to the oposite value
        }
    }

    return;
}