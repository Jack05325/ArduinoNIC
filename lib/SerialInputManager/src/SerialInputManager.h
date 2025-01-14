#ifndef SERIALINPUTMANAGER_H
#define SERIALINPUTMANAGER_H

#include <Arduino.h>
#include <StringSplitter.h>
#include "Layer3.h"
#include "Layer4.h"
#include "StackArray.h"

class SerialInputManager {
public:
    SerialInputManager(long baudRate, int timeout);
    void handleInput(String inputBuffer, StackArray<Pacchetto>* stackPacchetti, Layer3 *layer3, Layer4 *layer4);
private:
    void parseInputBuffer(StackArray<Pacchetto> *stackPacchetto, StringSplitter* splitter, Layer3 *layer3, Layer4 *layer4);
    String inputBuffer;
    StringSplitter *splitter = nullptr;
};

#endif 