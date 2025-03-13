#ifndef SERIALINPUTMANAGER_H
#define SERIALINPUTMANAGER_H

#include <Arduino.h>
#include <StringSplitter.h>
#include "Layer3.h"
#include "Layer4.h"
#include "Layer2.h"
#include "Layer1.h"
#include "StackArray.h"


class SerialInputManager {
public: 
    typedef struct {
        const char *command;
        void (*handler)(const char *args);
    } Command;
    SerialInputManager();
    void begin(Print* serial, Layer2 *layer2, Layer3 *layer3, Layer4 *layer4);
    void handleInput(StackArray<Pacchetto> *stackPacchetti, Command *commandTableFromUser = nullptr);
   
private:
    static SerialInputManager* _instance;
    void parseInputBuffer(StackArray<Pacchetto> *stackPacchetti);
    void processCommand(char *cmd);
    static void setIPAddrFromCommand(const char *IP);
    static void setMACAddrFromCommand(const char *MAC);
    static void helpCommand();
    //TODO: Definire i metodi per la gestione dei comandi da seriale
    Layer3 *_layer3 = nullptr;
    Layer4 *_layer4 = nullptr;
    Layer2 *_layer2 = nullptr;
    String inputBuffer;
    Print* _Serial;
    StringSplitter *_splitter = nullptr;
    Command *_commandTable;
    Command *_commandTableFromUser;
};

#endif 