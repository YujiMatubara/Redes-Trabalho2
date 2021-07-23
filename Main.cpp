#include "Sender.hpp"
#include "Receiver.hpp"
#include "Global.hpp"

int main(int argc, char const *argv[]) {
    Sender sender;
    Receiver receiver;

    sender.applicationLayer();
    vector<bool> senderFrame = sender.linkLayer(0); // can be 0, 1 or 2

    // communication path here...

    // receiver here...


    return 0;
}
