#include "Layer1.h"
#include "Arduino.h"
#include "StackArray.h"

const uint8_t DATA_SIZE = 16;
const uint8_t LAYER4_SIZE = 17;
const uint8_t LAYER3_SIZE = 11;
const uint8_t LAYER2_SIZE = 7;
const uint8_t LAYER1_SIZE = 2;

struct Pacchetto {
  uint8_t dati[DATA_SIZE];
  uint8_t layer4[LAYER4_SIZE];
  uint8_t layer3[LAYER3_SIZE];
  uint8_t layer2[LAYER2_SIZE];
  uint8_t layer1[LAYER1_SIZE];
};

Layer1::Layer1() {
  pinMode(TXPin, OUTPUT);
  pinMode(RXPin, INPUT);
}

bool Layer1::isLineaLibera() {
  return digitalRead(RXPin) == HIGH;
}

void Layer1::incapsulaDati(struct Pacchetto &pkt) {
   pkt.layer1[0] = preambolo >> 8;
   pkt.layer1[1] = (preambolo << 8) >> 8;
  //memcpy(pkt.layer1, &preambolo, LAYER1_SIZE);

  //Serial.print("Preambolo: ");
  //Serial.print(pkt.layer1[0]);
  //Serial.println(pkt.layer1[1]);
}

bool Layer1::checkPacchettoRicevuto(const Pacchetto pkt, Layer2 *layer2, Layer3 *layer3, Layer4 *layer4){
  return (layer2->checkPacketRecived(pkt) && layer3->checkPacketRecived(pkt) && layer4->checkPacketRecived(pkt));
}


void Layer1::transmitLayer(const uint8_t* data, size_t size) {
  for (size_t i = 0; i < size; ++i) {
    for (size_t j = 0; j < 8; ++j) {
      bool bit = bitRead(data[i], j);
      digitalWrite(TXPin, bit);
      delayMicroseconds(Tbit);
      Serial.print(bit);
    }
  }
}

void Layer1::inviaFrame(Pacchetto pkt) {
  transmitLayer(pkt.layer1, LAYER1_SIZE);
  transmitLayer(pkt.layer2, LAYER2_SIZE);
  transmitLayer(pkt.layer3, LAYER3_SIZE);
  transmitLayer(pkt.layer4, LAYER4_SIZE);
  transmitLayer(pkt.dati, DATA_SIZE);

  digitalWrite(TXPin, stopBit);
  delayMicroseconds(Tbit);
  // Serial.print(stopBit);
} 

uint16_t Layer1::getPreambolo() {
  return preambolo;
}

void Layer1::inviaFrame(StackArray<Pacchetto> &stackPacchettiDaInviare, int numeroPacchettiDaInviare, int delay) {
  for (int i = 0; i < stackPacchettiDaInviare.count(); ++i) {
    inviaFrame(stackPacchettiDaInviare.pop());
    delayMicroseconds(delay);
  }
}

//Pacchetto Layer1::riceviFrame(Pacchetto pkt){
//  Pacchetto pktRicevuto;
//  for(int i = 0; i < (int) sizeof(pktRicevuto.layer1); i++){
//    for(int j = 0; j < 8; j++){
//      //bool bit = digitalRead(RXPin);
//      bool bit = bitRead(pkt.layer1[i], j);
//      bitWrite(pktRicevuto.layer1[i], j, bit);
//      delayMicroseconds(Tbit);
//    }
//  }
//  
//  for(int i = 0; i < (int) sizeof(pktRicevuto.layer2); i++){
//    for(int j = 0; j < 8; j++){
//      //bool bit = digitalRead(RXPin);
//      bool bit = bitRead(pkt.layer2[i], j);
//      bitWrite(pktRicevuto.layer2[i], j, bit);
//      delayMicroseconds(Tbit);
//    }
//  }
//
//  for(int i = 0; i < (int) sizeof(pktRicevuto.layer3); i++){
//    for(int j = 0; j < 8; j++){
//      //bool bit = digitalRead(RXPin);
//      bool bit = bitRead(pkt.layer3[i], j);
//      bitWrite(pktRicevuto.layer3[i], j, bit);
//      delayMicroseconds(Tbit);
//    }
//  }
//
//  for(int i = 0; i < (int) sizeof(pktRicevuto.layer4); i++){
//    for(int j = 0; j < 8; j++){
//      //bool bit = digitalRead(RXPin);
//      bool bit = bitRead(pkt.layer4[i], j);
//      bitWrite(pktRicevuto.layer4[i], j, bit);
//      delayMicroseconds(Tbit);
//    }
//  }
//
//  for(int i = 0; i < (int) sizeof(pktRicevuto.dati); i++){
//    for(int j = 0; j < 8; j++){
//      //bool bit = digitalRead(RXPin);
//      bool bit = bitRead(pkt.dati[i], j);
//      bitWrite(pktRicevuto.dati[i], j, bit);
//      delayMicroseconds(Tbit);
//    }
//  }
//  return pktRicevuto;
//}

Pacchetto Layer1::riceviFrame(){
    uint8_t SimPacchetto[51];
    byte buffer[(int) sizeof(Pacchetto ) - (int) sizeof(preambolo)];
    uint16_t preamboloRicevuto = 0;
    if (digitalRead(RXPin) == LOW){
        delayMicroseconds(Tbit/2);
        for(int i = 0; i < (int) sizeof(preambolo)*8; i++){
            bool bit = digitalRead(RXPin);
            bitWrite(preamboloRicevuto, i, bit);
            delayMicroseconds(Tbit);
        }
        preamboloRicevuto = ~preamboloRicevuto;
        if(preamboloRicevuto == preambolo){
            for(int i = 0; i < (int) sizeof(buffer); i++){
                for(int j = 0; j < 8; j++){
                    //bool bit = !digitalRead(RXPin);
                    bool bit = bitRead(SimPacchetto[i], j);
                    bitWrite(buffer[i], j, bit);
                    delayMicroseconds(Tbit);
                }
            }
        }
        else{
            Serial.println("Errore: Preambolo non corrisponde");
            return;
        }
    }
    Pacchetto pkt;
    memcpy(&pkt, buffer, (int) sizeof(buffer));
    return pkt;
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
