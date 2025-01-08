#ifndef LAYER1_H
#define LAYER1_H

#include "Arduino.h"

class Layer1 {
    public:
        Layer1();
        void incapsulaDati(struct Pacchetto &pkt);
        void inviaFrame(struct Pacchetto &pkt);
        struct Pacchetto riceviFrame();

        void setTempoPerBit(int tempo);
        int getTempoPerBit();

        void setStopBit(int stop);
        int getStopBit();

        void setTXPin(int pin);
        int getTXPin();
        
        void setRXPin(int pin);
        int getRXPin();
    private:
        uint16_t preambolo = 0xAA;
        int Tbit = 100;
        int stopBit = 1;
        int TXPin = 8;
        int RXPin = 7;

};

#endif 