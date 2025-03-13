#include "Layer3.h"

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

Layer3::Layer3() {}

void Layer3::incapsulaDati(struct Pacchetto& pkt) {
    //memcpy(pkt.layer3, IP_Destinazione, 4);
    //memcpy(pkt.layer3 + 4, IP, 4);
    pkt.layer3[0] = IP_Destinazione[0];
    pkt.layer3[1] = IP_Destinazione[1];
    pkt.layer3[2] = IP_Destinazione[2];
    pkt.layer3[3] = IP_Destinazione[3];

    pkt.layer3[4] = IP[0];
    pkt.layer3[5] = IP[1];
    pkt.layer3[6] = IP[2];
    pkt.layer3[7] = IP[3];

    pkt.layer3[8] = TTL;

    crcl3.reset();
    for (uint8_t i = 0; i < 9; ++i) {  
        crcl3.add(pkt.layer3[i]);
    }
    uint16_t crcVal = crcl3.calc();

    pkt.layer3[9] = crcVal >> 8;
    pkt.layer3[10] = crcVal & 0xFF00;

    /*for(int i = 0; i < 11; i++){
        Serial.print(pkt.layer3[i]);
    }
    Serial.println();*/
}

bool Layer3::checkPacketRecived(const struct Pacchetto pkt) {
    crcl3.reset();
    for (int i = 0; i < LAYER3_SIZE-2; ++i) {
        crcl3.add(pkt.layer3[i]);
    }
    uint16_t crcVal = crcl3.calc();
    uint16_t crcValRecived = pkt.layer3[9] << 8 | pkt.layer3[10];
    return crcVal == crcValRecived;
}

const uint8_t* Layer3::getIP_Destinazione() const {
    return IP_Destinazione;
}

void Layer3::setIP_Destinazione(uint8_t* ip) {
    memcpy(IP_Destinazione, ip, 4);
}

const uint8_t* Layer3::getIP() const {
    return IP;
}

void Layer3::setIP(uint8_t* ip) {
    memcpy(IP, ip, 4);
}

uint8_t Layer3::getTTL() const {
    return TTL;
}

void Layer3::setTTL(uint8_t ttl) {
    TTL = ttl;
}