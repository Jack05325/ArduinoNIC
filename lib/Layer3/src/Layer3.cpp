#include "Layer3.h"

struct Pacchetto
{
  uint8_t dati[16];
  uint8_t layer4[17];
  uint8_t layer3[11];
  uint8_t layer2[7];
  uint8_t layer1[2];
};

Layer3::Layer3() {

}

void Layer3::incapsulaDati(struct Pacchetto& ptk) {
    memcpy(ptk.layer3, IP_Destinazione, 4);
    memcpy(ptk.layer3 + 4, IP_Mittente, 4);

    ptk.layer3[8] = TTL;

    crcl3.reset();
    for (uint8_t i = 0; i < 9; ++i) {  
        crcl3.add(ptk.layer3[i]);
    }
    uint16_t crcVal = crcl3.calc();

    ptk.layer3[9] = crcVal >> 8;
    ptk.layer3[10] = crcVal & 0xFF00;
}

const uint8_t* Layer3::getIP_Destinazione() const {
    return IP_Destinazione;
}

void Layer3::setIP_Destinazione(uint8_t* ip) {
    memcpy(IP_Destinazione, ip, 4);
}

const uint8_t* Layer3::getIP_Mittente() const {
    return IP_Mittente;
}

void Layer3::setIP_Mittente(uint8_t* ip) {
    memcpy(IP_Mittente, ip, 4);
}

uint8_t Layer3::getTTL() const {
    return TTL;
}

void Layer3::setTTL(uint8_t ttl) {
    TTL = ttl;
}