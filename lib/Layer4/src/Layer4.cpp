#include "Layer4.h"

struct Pacchetto
{
  uint8_t dati[16];
  uint8_t layer4[17];
  uint8_t layer3[11];
  uint8_t layer2[7];
  uint8_t layer1[2];
};

Layer4::Layer4(){

}

void Layer4::incapsulaDati(struct Pacchetto &ptk)
{
    //ptk.layer4[0] = windowSize >> 8;
    //ptk.layer4[1] = (windowSize << 8) >> 8;
    memcpy(ptk.layer4, &windowSize, 2);

    ptk.layer4[2] = flags;

    //ptk.layer4[3] = ackNumber >> (32-8);
    //ptk.layer4[4] = (ackNumber << (32-24)) >> (32-8);
    //ptk.layer4[6]  = (ackNumber << (32-8)) >> (32-8);
    //ptk.layer4[5]  = (ackNumber << (32-16)) >> (32-8); 
    memcpy(ptk.layer4 + 5, &ackNumber, 4);

    //ptk.layer4[7]  = seqNumber >> (32-8);
    //ptk.layer4[8]  = (seqNumber << (32-24)) >> (32-8);
    //ptk.layer4[9]  = (seqNumber << (32-16)) >> (32-8);
    //ptk.layer4[10]  = (seqNumber << (32-8)) >> (32-8);
    memcpy(ptk.layer4 + 9, &seqNumber, 4);

    //ptk.layer4[11]  = portaMittente >> 8;
    //ptk.layer4[12]  = (portaMittente << 8) >> 8;  
    memcpy(ptk.layer4 + (4+4+1+8), &portaMittente, 2);

    //ptk.layer4[13]  = portaDestinazione >> 8;
    //ptk.layer4[14]  = (portaDestinazione << 8) >> 8; 
    memcpy(ptk.layer4 + (4+4+1+8+2), &portaDestinazione, 2);
    
    crcl4.reset();
    for(uint8_t i : ptk.layer4){
        crcl4.add(i);
    }
    uint16_t crcVal = crcl4.calc();
    //ptk.layer4[15] = crcVal >> 8;
    //ptk.layer4[16] = (crcVal << 8) >> 8;
    memcpy(ptk.layer4 + (4+4+1+8+2+2), &crcVal, 2);

}

//TODO: implementare protocolli di layer trasporto tipo TCP e UDP --> Parlare col prof su come funzionano per farlo

//Set e Get
void Layer4::setWindowSize(uint16_t size) {
    windowSize = size;
}

uint16_t Layer4::getWindowSize() const {
    return windowSize;
}

void Layer4::setFlags(uint8_t flag) {
    flags = flag;
}

uint8_t Layer4::getFlags() const {
    return flags;
}

void Layer4::setAckNumber(uint32_t number) {
    ackNumber = number;
}

uint32_t Layer4::getAckNumber() const {
    return ackNumber;
}

void Layer4::setSeqNumber(uint32_t number) {
    seqNumber = number;
}

uint32_t Layer4::getSeqNumber() const {
    return seqNumber;
}

void Layer4::setPortaMittente(uint16_t port) {
    portaMittente = port;
}

uint16_t Layer4::getPortaMittente() const {
    return portaMittente;
}

void Layer4::setPortaDestinazione(uint16_t port) {
    portaDestinazione = port;
}

uint16_t Layer4::getPortaDestinazione() const {
    return portaDestinazione;
}
