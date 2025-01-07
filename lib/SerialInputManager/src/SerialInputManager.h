#ifndef SERIALINPUTMANAGER_H
#define SERIALINPUTMANAGER_H

#include <Arduino.h>
#include <StringSplitter.h>

class SerialInputManager {
public:
    SerialInputManager(long baudRate);
    struct Pacchetto handleInput(struct Pacchetto &pkt);
private:
    void parseInputBuffer(StringSplitter* splitter, struct Pacchetto &pkt);
    String inputBuffer;
    StringSplitter *splitter = nullptr;
};

#endif 