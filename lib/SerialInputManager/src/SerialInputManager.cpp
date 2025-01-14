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

SerialInputManager::SerialInputManager(long baudRate, int timeout) {
    //Serial.begin(baudRate);
    //Serial.setTimeout(timeout);
}

void SerialInputManager::handleInput(String inputBuffer, StackArray<Pacchetto>* stackPacchetti, Layer3 *layer3, Layer4 *layer4){
    //inputBuffer = "";
    //inputBuffer = Serial.readString();
    //Serial.println(inputBuffer);
    splitter = new StringSplitter(inputBuffer, '|', 5);    
    parseInputBuffer(stackPacchetti, splitter, layer3, layer4);
    Serial.println(stackPacchetti->count());
}

void SerialInputManager::parseInputBuffer(StackArray<Pacchetto> *stackPacchetti ,StringSplitter *splitter, Layer3* layer3, Layer4 *layer4){
    String IP_Dest = splitter->getItemAtIndex(0);
    int length = (16 + splitter->getItemAtIndex(4).length() - 1) / 16;    
    Serial.print("L 5: ");
    Serial.println(length);
    Pacchetto pktTmp;

    //!Creo lo stack di pacchetti inserendo solo i dati, poichè il layer 3 e 4 sono uguali per tutti i pacchetti e lo fa nel main --> possibile modifica parlare con il prof
    for(int i = 0; i < length; i++){
        memcpy(pktTmp.dati, splitter->getItemAtIndex(4).substring(i*16, ((i+1)*16)).c_str(), 16);
        stackPacchetti->push(pktTmp);
        Serial.println(splitter->getItemAtIndex(4).substring(i*16, (((i+1)*16))));
        Serial.println(stackPacchetti->count());
    }
    layer3->setTTL(splitter->getItemAtIndex(1).toInt());
    layer4->setPortaDestinazione((uint16_t)splitter->getItemAtIndex(2).toInt());
    layer4->setPortaMittente((uint16_t)splitter->getItemAtIndex(3).toInt()); 

    //Serial.println(IP_Dest);
    //Serial.println(splitter->getItemAtIndex(1));
    //Serial.println(splitter->getItemAtIndex(2).toInt(), HEX);
    //Serial.println(splitter->getItemAtIndex(3).toInt(), HEX);
    //Serial.println(splitter->getItemAtIndex(4));

    delete splitter;

    splitter = new StringSplitter(IP_Dest, '.', 4);
    uint8_t IP[4];
    for(int i = 0; i < splitter->getItemCount(); i++){
        IP[i] = splitter->getItemAtIndex(i).toInt();
    }
    layer3->setIP_Destinazione(IP);

    delete splitter;
}