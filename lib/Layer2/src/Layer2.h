#ifndef LAYER2_H
#define LAYER2_H

#include <Arduino.h>
#include "CRC16.h"
#include "CRC.h"

class Layer2 {
    public:
        Layer2();
        void incapsulaDati(struct Pacchetto &pkt);

        uint16_t getMAC_Destinazione() const;
        void setMAC_Destinazione(uint16_t mac);

        uint16_t getMAC_Mittente() const;
        void setMAC_Mittente(uint16_t mac);

        uint8_t getProtocolType() const;
        void setProtocolType(uint8_t type);

    private:
        uint16_t MAC_Destinazione = 0x1234; 
        uint16_t MAC_Mittente = 0x1234;
        uint8_t protocolType = 0x12;
        CRC16 crcl2;
};

#endif 