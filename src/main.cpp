#include <Arduino.h>
#include <StackArray.h>

#include "SerialInputManager.h"
#include "Layer4.h"
#include "Layer3.h"
#include "Layer2.h"
#include "Layer1.h"

const uint8_t DATA_SIZE = 16;
const uint8_t LAYER4_SIZE = 17;
const uint8_t LAYER3_SIZE = 11;
const uint8_t LAYER2_SIZE = 7;
const uint8_t LAYER1_SIZE = 2;

struct Pacchetto{
  uint8_t dati[DATA_SIZE];
  uint8_t layer4[LAYER4_SIZE];
  uint8_t layer3[LAYER3_SIZE];
  uint8_t layer2[LAYER2_SIZE];
  uint8_t layer1[LAYER1_SIZE];
};


StackArray<Pacchetto> stackPacchetti;
StackArray<Pacchetto> stackPacchettiDaInviare;

Layer4 layer4;
Layer3 layer3;
Layer2 layer2;
Layer1 layer1;

SerialInputManager sim;



void setup()
{
  Serial.begin(115200);
  sim.begin(&Serial);
  stackPacchetti.setPrinter(Serial);
  stackPacchettiDaInviare.setPrinter(Serial);
}

char buffer[100] = {0};
StackArray<Pacchetto> stackPacchettiRXtest;


void loop()
{
  while (Serial.available() == 0) {
    if(layer1.isLineaLibera() && !stackPacchettiDaInviare.isEmpty()){
      Serial.println("Linea libera");
      layer1.inviaFrame(stackPacchettiDaInviare, stackPacchettiDaInviare.count(), 100);
    }
    else{
      Pacchetto pkt;
      pkt = layer1.riceviFrame();
      layer1.checkPacchettoRicevuto(pkt, &layer2, &layer3, &layer4);
    }
  }
  
  Serial.readBytes(buffer, 100);
  Serial.println(buffer);
  
  sim.handleInput(buffer, &stackPacchetti, &layer3, &layer4);
  
  memset(buffer, 0, sizeof(buffer));
  //handleUDP();
  
  Serial.clearWriteError();
  Serial.flush();
}



/*
!UDP --> Test di impletazione si async nella interfaccia python invece di getione da arduino 
void handleUDP(){  

  Serial.println("Received");
  for(int i = 0; i < stackPacchetti.count(); i++){
    Pacchetto pkt = stackPacchetti.pop();    
    layer4.incapsulaDati(pkt);
    layer3.incapsulaDati(pkt);
    layer2.incapsulaDati(pkt);
    layer1.incapsulaDati(pkt);
    stackPacchetti.unshift(pkt);
    Serial.println(i);
  }

  //layer1->inviaFrame(stackPacchetti, stackPacchetti.count(), 0);
  
  //if(layer1->isLineaLibera()){
  //  Serial.println("Linea libera");
  //  layer1->inviaFrame(stackPacchetti, stackPacchetti.count(), 0);
  //}
  //else{
  //  Serial.println("Linea occupata");
  //  for(int i = 0; i < stackPacchetti.count()+1; i++){
  //    Pacchetto pkt = stackPacchetti.pop();
  //    stackPacchettiDaInviare.unshift(pkt);
  //  }
  //}
  Serial.println("L stack pacchetti da inviare: " + String(stackPacchettiDaInviare.count()));
  Serial.println("L stack pacchetti UDP: " + String(stackPacchetti.count()));
  Serial.println("Test invio");
  int lStack = stackPacchetti.count();
  for(int i = 0; i < lStack; i++){
    Pacchetto pkt = stackPacchetti.pop();
    stackPacchettiRXtest.unshift(pkt);
    layer1.inviaFrame(pkt);
  }
  Serial.println("Test ricezione");
  for(int i = 0; i < lStack; i ++){
    Pacchetto pktRX = layer1.riceviFrame();
    layer1.inviaFrame(pktRX);
    Serial.println("Ricevuto");
  }
  Serial.println("UDP Finito");

}
//!TCP
//int TmaxForHandshake = 100; 
//int flags[] = {
//  0b00000001,//SYN
//  0b00000010,//ACK
//  0b00000100,//DATA
//  0b00001000,//FIN
//  0b00010000,//RST
//};
//
//void handleTCPTx(){
//  layer4->setFlags(flags[0]);
//  
*/
//}