#ifndef LAYER1_H
#define LAYER1_H

#include "Arduino.h"
#include "StackArray.h"


class Layer1 {
    public:
        Layer1();
        void incapsulaDati(struct Pacchetto &pkt);
        void inviaFrame(Pacchetto stackPacchetti); //*Invia un singolo pacchetto
        void inviaFrame(StackArray<Pacchetto> stackPacchetti, int numeroPacchettiDaInviare, int delay); //*Invia un numero di pacchetti specificato con un delay(in microsecondi) tra un pacchetto e l'altro
        bool isLineaLibera();

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
        void transmitLayer(const uint8_t *layer, size_t size);
        uint16_t preambolo = 0xAA;
        int Tbit = 100;
        int stopBit = 1;
        int TXPin = 8;
        int RXPin = 7;

};

#endif 