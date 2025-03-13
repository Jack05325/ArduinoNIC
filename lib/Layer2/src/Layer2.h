#ifndef LAYER2_H
#define LAYER2_H

#include <Arduino.h>
#include "CRC16.h"
#include "CRC.h"

class Layer2 {
    public:
        Layer2();
        void incapsulaDati(struct Pacchetto &pkt);

        const uint8_t* getMAC_Destinazione() const;
        void setMAC_Destinazione(const uint8_t *mac);

        const uint8_t* getMAC() const;
        void setMAC(const uint8_t *mac);

        uint8_t getProtocolType() const;
        void setProtocolType(uint8_t type);

        bool checkPacketRecived (const struct Pacchetto pkt);

    private:
        uint8_t MAC_Destinazione[2] = {0x00, 0x00}; 
        uint8_t MAC[2] = {0x00, 0x00};
        uint8_t protocolType = 0x12;
        CRC16 crcl2;
};

#endif 