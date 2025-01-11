#ifndef LAYER3_H
#define LAYER3_H

#include <Arduino.h>
#include "CRC16.h"
#include "CRC.h"


class Layer3 {
public:
    Layer3();
    void incapsulaDati(struct Pacchetto& pkt);

    const uint8_t* getIP_Destinazione() const;
    void setIP_Destinazione(uint8_t* ip);

    const uint8_t* getIP_Mittente() const;
    void setIP_Mittente(uint8_t* ip);

    uint8_t getTTL() const;
    void setTTL(uint8_t ttl);

private:
    uint8_t IP_Destinazione[4] = {0, 0, 0, 0};
    uint8_t IP_Mittente[4]     = {0, 0, 0, 0};
    uint8_t TTL                = 0x00;
    CRC16 crcl3;
};

#endif
