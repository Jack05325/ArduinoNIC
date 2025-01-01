#include "Layer3.h"

struct pacchetto
{
  uint8_t dati[16];
  uint8_t layer4[17];
  uint8_t layer3[11];
  uint8_t layer2[7];
  uint8_t layer1[2];
};

Layer3::Layer3(){}


void Layer3::incapsulaDati(struct pacchetto &ptk){
    ptk.layer3[0] = IP_Destinazione >> (32-8);
    ptk.layer3[1] = (IP_Destinazione << (32-24)) >> (32-8);
    ptk.layer3[2]  = (IP_Destinazione << (32-16)) >> (32-8); 
    ptk.layer3[3]  = (IP_Destinazione << (32-8)) >> (32-8);

    ptk.layer3[4] = IP_Mittente >> (32-8);
    ptk.layer3[5] = (IP_Mittente << (32-24)) >> (32-8);
    ptk.layer3[6] = (IP_Mittente << (32-16)) >> (32-8); 
    ptk.layer3[7] = (IP_Mittente << (32-8)) >> (32-8);

    ptk.layer3[8] =  TTL;

    crcl3.reset();
    for(uint8_t i : ptk.layer3){
        crcl3.add(i);
    }
    uint16_t crcVal = crcl3.calc();
    ptk.layer3[9] = crcVal >> 8;
    ptk.layer3[10] = (crcVal << 8) >> 8;
}

uint32_t Layer3::getIP_Destinazione() const {
    return IP_Destinazione;
}

void Layer3::setIP_Destinazione(uint32_t ip) {
    IP_Destinazione = ip;
}

uint32_t Layer3::getIP_Mittente() const {
    return IP_Mittente;
}

void Layer3::setIP_Mittente(uint32_t ip) {
    IP_Mittente = ip;
}

uint8_t Layer3::getTTL() const {
    return TTL;
}

void Layer3::setTTL(uint8_t ttl) {
    TTL = ttl;
}