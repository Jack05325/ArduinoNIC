#ifndef LAYER3_H
#define LAYER3_H

#include <Arduino.h>
#include "CRC16.h"
#include "CRC.h"

class Layer3{
    public:
        Layer3();
        void incapsulaDati(struct pacchetto &ptk);
        
        uint32_t getIP_Destinazione() const;
        void setIP_Destinazione(uint32_t ip);

        uint32_t getIP_Mittente() const;
        void setIP_Mittente(uint32_t ip);

        uint8_t getTTL() const;
        void setTTL(uint8_t ttl);
    private:
        uint32_t IP_Destinazione = 0x12345678;
        uint32_t IP_Mittente     = 0x12345678;
        uint8_t TTL              = 0x12;
        CRC16 crcl3;
};

#endif