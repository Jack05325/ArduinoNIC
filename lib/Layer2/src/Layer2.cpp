#include "Layer2.h"

struct Pacchetto
{
  uint8_t dati[16];
  uint8_t layer4[17];
  uint8_t layer3[11];
  uint8_t layer2[7];
  uint8_t layer1[2];
};


Layer2::Layer2() {
}

void Layer2::incapsulaDati(struct Pacchetto &ptk){
    //ptk.layer2[0] = MAC_Destinazione >> 8;
    //ptk.layer2[1] = (MAC_Destinazione << 8) >> 8;
    memcpy(ptk.layer2, &MAC_Destinazione, 2);

    //ptk.layer2[2] = MAC_Mittente >> 8;
    //ptk.layer2[3] = (MAC_Mittente << 8) >> 8//;
    memcpy(ptk.layer2 + 2, &MAC_Mittente, 2);

    //ptk.layer2[4] = protocolType;
    memcpy(ptk.layer2 + 4, &protocolType, 1);

    crcl2.reset();
    for(uint8_t i : ptk.layer3){
        crcl2.add(i);
    }
    uint16_t crcVal = crcl2.calc();
    //ptk.layer2[5] = crcVal >> 8;
    //ptk.layer2[6] = (crcVal << 8) >> 8;
    memcpy(ptk.layer2 + (2+2+1), &crcVal, 2);
}

uint16_t Layer2::getMAC_Destinazione() const {
    return MAC_Destinazione;
}

void Layer2::setMAC_Destinazione(uint16_t macDestinazione) {
    MAC_Destinazione = macDestinazione;
}

uint16_t Layer2::getMAC_Mittente() const {
    return MAC_Mittente;
}

void Layer2::setMAC_Mittente(uint16_t macMittente) {
    MAC_Mittente = macMittente;
}

uint8_t Layer2::getProtocolType() const {
    return protocolType;
}

void Layer2::setProtocolType(uint8_t protocolType) {
    this->protocolType = protocolType;
}