#ifndef LAYER4_H
#define LAYER4_H

#include <Arduino.h>
#include "CRC16.h"
#include "CRC.h"

class Layer4{
    public:
        Layer4();
        void incapsulaDati(struct pacchetto &ptk);
        void setWindowSize(uint16_t _windowSize);
        void setFlags(uint8_t _flags);
        void setAckNumber(uint32_t _ackNumber);
        void setSeqNumber(uint32_t _seqNumber);
        void setPortaMittente(uint16_t _portaMittente);
        void setPortaDestinazione(uint16_t _portaDestinazione);
        uint16_t getWindowSize() const;
        uint8_t getFlags() const;
        uint32_t getAckNumber() const;
        uint32_t getSeqNumber() const;
        uint16_t getPortaMittente() const;
        uint16_t getPortaDestinazione() const;
    private:
    uint16_t windowSize = 0x1234;
    uint8_t flags = 0x12;
    uint32_t ackNumber = 0x12345678;
    uint32_t seqNumber = 0x12345678;
    uint16_t portaMittente = 0x1234;
    uint16_t portaDestinazione = 0x1234;
    CRC16 crcl4;
};

#endif