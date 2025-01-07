#include "SerialInputManager.h"
#include "Arduino.h"

struct Pacchetto
{
  uint8_t dati[16];
  uint8_t layer4[17];
  uint8_t layer3[11];
  uint8_t layer2[7];
  uint8_t layer1[2];
};

SerialInputManager::SerialInputManager(long baudRate) {
    Serial.begin(baudRate);
    Serial.setTimeout(1);
}

//!Controllare eventuali memory leak per evitare attachi --> anche se magari potrebbe essere interessante per sperimente diversi tipi di attacchi di cyber security
struct Pacchetto SerialInputManager::handleInput(struct Pacchetto &pkt){
    inputBuffer = "";
    inputBuffer = Serial.readString();
    delete splitter;
    splitter = new StringSplitter(inputBuffer, '|', 5);
    parseInputBuffer(splitter, pkt);
}

void SerialInputManager::parseInputBuffer(StringSplitter* splitter, struct Pacchetto &pkt){
    String IP_Dest = splitter->getItemAtIndex(0);
    pkt.layer3[0]  = splitter->getItemAtIndex(1).toInt();
    pkt.layer4[0]  = splitter->getItemAtIndex(2).toInt();
    pkt.layer4[1]  = splitter->getItemAtIndex(3).toInt();
    memcpy(pkt.dati, splitter->getItemAtIndex(4).c_str(), 16);
    delete splitter;

    splitter = new StringSplitter(IP_Dest, '.', 4);
    for(int i = 0; i < splitter->getItemCount(); i++){
        pkt.layer3[i+1] = byte(splitter->getItemAtIndex(i).toInt());
    }
    delete splitter;
}