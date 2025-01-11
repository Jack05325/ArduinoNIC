#include "Layer4.h"

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

Layer4::Layer4(){

}

void Layer4::incapsulaDati(struct Pacchetto &pkt)
{
    pkt.layer4[0] = windowSize >> 8;
    pkt.layer4[1] = (windowSize << 8) >> 8;
    //memcpy(pkt.layer4, &windowSize, 2);

    pkt.layer4[2] = flags;
    //memcpy(pkt.layer4 + 2, &flags, 1);

    pkt.layer4[3] = ackNumber >> (32-8);
    pkt.layer4[4] = (ackNumber << (32-24)) >> (32-8);
    pkt.layer4[6]  = (ackNumber << (32-8)) >> (32-8);
    pkt.layer4[5]  = (ackNumber << (32-16)) >> (32-8); 
    //memcpy(pkt.layer4 + (2+1), &ackNumber, 4);

    pkt.layer4[7]  = seqNumber >> (32-8);
    pkt.layer4[8]  = (seqNumber << (32-24)) >> (32-8);
    pkt.layer4[9]  = (seqNumber << (32-16)) >> (32-8);
    pkt.layer4[10]  = (seqNumber << (32-8)) >> (32-8);
    //memcpy(pkt.layer4 + (2+1+4), &seqNumber, 4);

    pkt.layer4[11]  = portaMittente >> 8;
    pkt.layer4[12]  = (portaMittente << 8) >> 8;  
    //memcpy(pkt.layer4 + (2+1+4+4), &portaMittente, 2);

    pkt.layer4[13]  = portaDestinazione >> 8;
    pkt.layer4[14]  = (portaDestinazione << 8) >> 8; 
    //memcpy(pkt.layer4 + (2+1+4+4+2), &portaDestinazione, 2);
    
    crcl4.reset();
    for(uint8_t i : pkt.layer4){
        crcl4.add(i);
    }
    uint16_t crcVal = crcl4.calc();
    pkt.layer4[15] = crcVal >> 8;
    pkt.layer4[16] = (crcVal << 8) >> 8;
    //memcpy(pkt.layer4 + (2+1+4+4+2+2), &crcVal, 2);
    
    /*for(int i = 0; i < 17; i++){
        Serial.print(pkt.layer4[i], HEX);
    }
    Serial.println();*/
}


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
