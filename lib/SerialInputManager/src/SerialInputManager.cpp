#include "SerialInputManager.h"
#include "Arduino.h"
#include "Layer3.h"
#include "Layer4.h"
#include "StackArray.h"

const uint8_t DATA_SIZE = 16;
const uint8_t LAYER4_SIZE = 17;
const uint8_t LAYER3_SIZE = 11;
const uint8_t LAYER2_SIZE = 7;
const uint8_t LAYER1_SIZE = 2;

struct Pacchetto {
  uint8_t dati[DATA_SIZE];
  uint8_t layer4[LAYER4_SIZE];
  uint8_t layer3[LAYER3_SIZE];
  uint8_t layer2[LAYER2_SIZE];
  uint8_t layer1[LAYER1_SIZE];
};

typedef struct {
  const char *command;
  void (*handler)(const char *args);
} Command;

SerialInputManager::SerialInputManager() {
    _instance = this;
    Command *_commandTable;
    _commandTable = new Command[4] {
        {"set-ip-addr", [](const char *args) { setIPAddrFromCommand(args); }},
        {"set-mac-addr", [](const char *args) {setMACAddrFromCommand(args); }},
        {"help", [](const char *){helpCommand(); }},
        {NULL, NULL}
    };
}

void SerialInputManager::helpCommand(){
    _instance->_Serial->println(F(
        " __    __  ________  __        _______         __       __  ________  __    __  __    __ \n"
        "/  |  /  |/        |/  |      /       \\       /  \\     /  |/        |/  \\  /  |/  |  /  |\n"
        "$$ |  $$ |$$$$$$$$/ $$ |      $$$$$$$  |      $$  \\   /$$ |$$$$$$$$/ $$  \\ $$ |$$ |  $$ |\n"
        "$$ |__$$ |$$ |__    $$ |      $$ |__$$ |      $$$  \\ /$$$ |$$ |__    $$$  \\$$ |$$ |  $$ |\n"
        "$$    $$ |$$    |   $$ |      $$    $$/       $$$$  /$$$$ |$$    |   $$$$  $$ |$$ |  $$ |\n"
        "$$$$$$$$ |$$$$$/    $$ |      $$$$$$$/        $$ $$ $$/$$ |$$$$$/    $$ $$ $$ |$$ |  $$ |\n"
        "$$ |  $$ |$$ |_____ $$ |_____ $$ |            $$ |$$$/ $$ |$$ |_____ $$ |$$$$ |$$ \\__$$ |\n"
        "$$ |  $$ |$$       |$$       |$$ |            $$ | $/  $$ |$$       |$$ | $$$ |$$    $$/ \n"
        "$$/   $$/ $$$$$$$$/ $$$$$$$$/ $$/             $$/      $$/ $$$$$$$$/ $$/   $$/  $$$$$$/  \n"
        "\n"
        "- set-ip-addr <Indirizzo ip> --> imposta l'indirizzo IP\n"
        "- set-mac-addr <Indirizzo mac> --> imposta l'indirizzo MAC\n"
        ));
}

void SerialInputManager::setIPAddrFromCommand(const char* IP){
    _instance->_splitter = new StringSplitter(IP, '.', 4);
    uint8_t IP_to_set[4];
    for(int i = 0; i < _instance->_splitter->getItemCount(); i++){
        IP_to_set[i] = _instance->_splitter->getItemAtIndex(i).toInt();
    }
    _instance->_layer3->setIP(IP_to_set);
}

void SerialInputManager::setMACAddrFromCommand(const char* MAC){
    _instance->_splitter = new StringSplitter(MAC, '.', 2);
    uint8_t MAC_TO_SET[2];
    MAC_TO_SET[0] = _instance->_splitter->getItemAtIndex(0).toInt();
    MAC_TO_SET[1] = _instance->_splitter->getItemAtIndex(1).toInt();
    _instance->_layer2->setMAC(MAC_TO_SET);
}

void SerialInputManager::begin(Print* serial, Layer2 *layer2, Layer3 *layer3, Layer4 *layer4){
    _Serial = serial;
    _layer2 = layer2;
    _layer3 = layer3;
    _layer4 = layer4;
}

void SerialInputManager::handleInput(StackArray<Pacchetto> *stackPacchetti, Command *commandTableFromUser = NULL){
    _commandTableFromUser = commandTableFromUser;
    inputBuffer = "";
    inputBuffer = Serial.readString();
    Serial.println(inputBuffer);
    _splitter = new StringSplitter(inputBuffer, '|', 6);    
    parseInputBuffer(stackPacchetti);
    _Serial->println(stackPacchetti->count());
}

//TODO: Sistemare il parsing dei dati perché sarà in parte gestiono dalla interfaccia seriale
void SerialInputManager::parseInputBuffer(StackArray<Pacchetto> *stackPacchetti){
    String IP_Dest = _splitter->getItemAtIndex(0);
    int length = (16 + _splitter->getItemAtIndex(4).length() - 1) / 16;    
    _Serial->print("L 5: ");
    _Serial->println(length);
    Pacchetto pktTmp;

    //!Creo lo stack di pacchetti inserendo solo i dati, poichè il layer 3 e 4 sono uguali per tutti i pacchetti e lo fa nel main --> possibile modifica parlare con il prof
    for(int i = 0; i < length; i++){
        memcpy(pktTmp.dati, _splitter->getItemAtIndex(4).substring(i*16, ((i+1)*16)).c_str(), 16);
        stackPacchetti->push(pktTmp);
        Serial.println(_splitter->getItemAtIndex(4).substring(i*16, (((i+1)*16))));
        Serial.println(stackPacchetti->count());
    }
    _layer3->setTTL(_splitter->getItemAtIndex(1).toInt());
    _layer4->setPortaDestinazione((uint16_t)_splitter->getItemAtIndex(2).toInt());
    _layer4->setPortaMittente((uint16_t)_splitter->getItemAtIndex(3).toInt()); 

    //Serial.println(IP_Dest);
    //Serial.println(splitter->getItemAtIndex(1));
    //Serial.println(splitter->getItemAtIndex(2).toInt(), HEX);
    //Serial.println(splitter->getItemAtIndex(3).toInt(), HEX);
    //Serial.println(splitter->getItemAtIndex(4));

    delete _splitter;

    _splitter = new StringSplitter(IP_Dest, '.', 4);
    uint8_t IP[4];
    for(int i = 0; i < _splitter->getItemCount(); i++){
        IP[i] = _splitter->getItemAtIndex(i).toInt();
    }
    _layer3->setIP_Destinazione(IP);

    delete _splitter;
}

void SerialInputManager::processCommand(char *cmd) {
    char *command = strtok(cmd, " "); // Estrai il comando
    char *args = strtok(NULL, ""); // Estrai gli argomenti
    
    if (command) {
        for (int i = 0; _commandTable[i].command != NULL; i++) {
            if (strcmp(command, _commandTable[i].command) == 0) {
                _commandTable[i].handler(args ? args : "");
                return;
            }
        }
        for (int i = 0; i < sizeof(_commandTableFromUser)/sizeof(Command); i++){
            if (strcmp(command, _commandTableFromUser[i].command) == 0) {
                _commandTableFromUser[i].handler(args ? args : "");
                return;
            }
        }
    }
}