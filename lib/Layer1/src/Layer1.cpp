#include "layer1.h"

struct pacchetto
{
  uint8_t dati[16];
  uint8_t layer4[17];
  uint8_t layer3[11];
  uint8_t layer2[7];
  uint8_t layer1[2];
};

Layer1::Layer1() {

}

void Layer1::incapsulaDati(struct  pacchetto &pkt) {
    pkt.layer1[0] = preambolo >> 8;
    pkt.layer1[1] = (preambolo << 8) >> 8;
}

void Layer1::inviaPacchetto(struct pacchetto &pkt) {

}

void Layer1::setTempoPerBit(int tempo) {
    Tbit = tempo;
}

int Layer1::getTempoPerBit() {
    return Tbit;
}