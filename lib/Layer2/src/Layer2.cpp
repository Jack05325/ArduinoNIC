#include "Layer2.h"

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


Layer2::Layer2() {
}

void Layer2::incapsulaDati(struct Pacchetto &pkt){
    pkt.layer2[0] = MAC_Destinazione >> 8;
    pkt.layer2[1] = (MAC_Destinazione << 8) >> 8;
    //memcpy(pkt.layer2, &MAC_Destinazione, 2);

    pkt.layer2[2] = MAC_Mittente >> 8;
    pkt.layer2[3] = (MAC_Mittente << 8) >> 8;
    //memcpy(pkt.layer2 + 2, &MAC_Mittente, 2);

    pkt.layer2[4] = protocolType;
    //memcpy(pkt.layer2 + 4, &protocolType, 1);

    crcl2.reset();
    for(uint8_t i : pkt.layer3){
        crcl2.add(i);
    }
    uint16_t crcVal = crcl2.calc();
    pkt.layer2[5] = crcVal >> 8;
    pkt.layer2[6] = (crcVal << 8) >> 8;
    //memcpy(pkt.layer2 + (2+2+1), &crcVal, 2);
    
    /*for(int i = 0; i < 7; i++){
        Serial.print(pkt.layer2[i], HEX);
    }
    Serial.println();*/
}

bool Layer2::checkPacketRecived(const struct Pacchetto pkt){
    crcl2.reset();
    for(int i = 0; i < LAYER2_SIZE-2; i++){
        crcl2.add(pkt.layer2[i]);
    }
    uint16_t crcVal = crcl2.calc();
    uint16_t crcValRecived = pkt.layer2[5] << 8 | pkt.layer2[6];
    return crcVal == crcValRecived;
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