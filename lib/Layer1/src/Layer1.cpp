#include "layer1.h"
#include "Arduino.h"

struct Pacchetto
{
  uint8_t dati[16];
  uint8_t layer4[17];
  uint8_t layer3[11];
  uint8_t layer2[7];
  uint8_t layer1[2];
};

Layer1::Layer1() {

}

void Layer1::incapsulaDati(struct  Pacchetto &pkt) {
//    pkt.layer1[0] = preambolo >> 8;
//    pkt.layer1[1] = (preambolo << 8) >> 8;
    memcpy(pkt.layer1, &preambolo, 2);
}

void Layer1::inviaFrame(struct Pacchetto &pkt) {
    //!Invio Layer 1
    for(int i = 0; i < sizeof(pkt.layer1); i++){
        for(int j = 0; j < 8; j++){
            bool bit = bitRead(pkt.layer1[i], j);
            digitalWrite(TXPin, bit);
            delayMicroseconds(Tbit);
            Serial.print(bit);
        }
    }
    //!Invio Layer 2
    for(int i = 0; i < sizeof(pkt.layer2); i++){
        for(int j = 0; j < 8; j++){
            bool bit = bitRead(pkt.layer2[i], j);
            digitalWrite(TXPin, bit);
            delayMicroseconds(Tbit);
            Serial.print(bit);
        }
    }
    //!Invio Layer 3
    for(int i = 0; i < sizeof(pkt.layer3); i++){
        for(int j = 0; j < 8; j++){
            bool bit = bitRead(pkt.layer3[i], j);
            digitalWrite(TXPin, bit);
            delayMicroseconds(Tbit);
            Serial.print(bit);
        }
    }
    //!Invio Layer 4
    for(int i = 0; i < sizeof(pkt.layer4); i++){
        for(int j = 0; j < 8; j++){
            bool bit = bitRead(pkt.layer4[i], j);
            digitalWrite(TXPin, bit);
            delayMicroseconds(Tbit);
            Serial.print(bit);
        }
    }
    //!Invio Dati
    for(int i = 0; i < sizeof(pkt.dati); i++){
        for(int j = 0; j < 8; j++){
            bool bit = bitRead(pkt.dati[i], j);
            digitalWrite(TXPin, bit);
            delayMicroseconds(Tbit);
            Serial.print(bit);
        }
    }
    //!Invio Bit di STOP
    digitalWrite(TXPin, stopBit);
    delayMicroseconds(Tbit);
    Serial.print(stopBit);
}

struct Pacchetto Layer1::riceviFrame(){
    
}

void Layer1::setTempoPerBit(int tempo) {
    Tbit = tempo;
}

int Layer1::getTempoPerBit() {
    return Tbit;
}



void Layer1::setStopBit(int stop) {
    stopBit = stop;
}

int Layer1::getStopBit() {
    return stopBit;
}



void Layer1::setTXPin(int pin) {
    TXPin = pin;
}

int Layer1::getTXPin() {
    return TXPin;
}



void Layer1::setRXPin(int pin) {
    RXPin = pin;
}

int Layer1::getRXPin() {
    return RXPin;
}
