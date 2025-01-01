#ifndef LAYER1_H
#define LAYER1_H

#include "Arduino.h"

class Layer1 {
    public:
        Layer1();
        void incapsulaDati(struct pacchetto &pkt);
        void inviaPacchetto(struct pacchetto &pkt);

        void setTempoPerBit(int tempo);
        int getTempoPerBit();

        void setStopBit(int stop);
        int getStopBit();
    private:
        uint16_t preambolo = 0xAA;
        int Tbit = 100;
        int stopBit = 1;

};

#endif 