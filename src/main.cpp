#include <Arduino.h>
#include "TaskScheduler.h"
#include <StackArray.h>

#include "SerialInputManager.h"
#include "Layer4.h"
#include "Layer3.h"
#include "Layer2.h"
#include "Layer1.h"

#include "CRC16.h"
#include "CRC.h"

CRC16 crc;

Layer4 *layer4 = new Layer4();
Layer3 *layer3 = new Layer3();
Layer2 *layer2 = new Layer2();
Layer1 *layer1 = new Layer1();

bool core1_separate_stack = true;

SerialInputManager sim(115200, 2);


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


StackArray<Pacchetto> stackPacchetti;
StackArray<Pacchetto> stackPacchettiDaInviare;

void setup()
{
  Serial.begin(115200);
  Serial.setTimeout(2);
  stackPacchetti.setPrinter(Serial);
  stackPacchettiDaInviare.setPrinter(Serial);
}



void handleUDP(){
  sim.handleInput(&stackPacchetti, layer3, layer4);

  for(int i = 0; i < stackPacchetti.count(); i++){
    Pacchetto pkt = stackPacchetti.pop();    
    layer4->incapsulaDati(pkt);
    layer3->incapsulaDati(pkt);
    layer2->incapsulaDati(pkt);
    layer1->incapsulaDati(pkt);
    stackPacchetti.unshift(pkt);
  }

  layer1->inviaFrame(stackPacchetti, stackPacchetti.count(), 0);

  if(layer1->isLineaLibera()){
    Serial.println("Linea libera");
    layer1->inviaFrame(stackPacchetti, stackPacchetti.count(), 100);
  }
  else{
    for(int i = 0; i < stackPacchetti.count(); i++){
      stackPacchettiDaInviare.unshift(stackPacchetti.pop());
    }
  }
}

void loop()
{
  while (!Serial.available()) {
    if(layer1->isLineaLibera() && !stackPacchettiDaInviare.isEmpty()){
      Serial.println("Linea libera");
      layer1->inviaFrame(stackPacchettiDaInviare, stackPacchettiDaInviare.count(), 100);
    }
  }
  Serial.println("Received");

  handleUDP();


}