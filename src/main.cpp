#include <Arduino.h>
#include "TaskScheduler.h"

#include "SerialInputManager.h"
#include "Layer4.h"
#include "Layer3.h"
#include "Layer2.h"
#include "Layer1.h"

#include "CRC16.h"
#include "CRC.h"

CRC16 crc;

Layer4 layer4;
Layer3 layer3;
Layer2 layer2;
Layer1 layer1;

SerialInputManager sim(115200);

int pullUP = 7;
int linea = 8;
int start = 0;
int stop = 1;
int Tbit = 100;

int id = 5;      // "IP" del mittente
char dato = 'a'; // TX
int idDest = 5;

int idDestRX = 0; // RX
int idMitt;
int idBroadcast = 15;
int butt = 4;


struct Pacchetto
{
  uint8_t dati[16];
  uint8_t layer4[17];
  uint8_t layer3[11];
  uint8_t layer2[7];
  uint8_t layer1[2];
};

uint8_t data[16];
uint8_t IP_Dest[4];

struct Pacchetto pkt;
int dataType;
uint8_t frameRX[sizeof(Pacchetto)];

void setup()
{
  pinMode(linea, INPUT);
  pinMode(pullUP, OUTPUT);
  Serial.begin(115200);
  Serial.setTimeout(2);
  pinMode(butt, INPUT_PULLUP);
  digitalWrite(linea, HIGH);
}


void loop()
{
  while (!Serial.available())
  {
    if (digitalRead(linea) == LOW)
    {

    }
  }
  if (digitalRead(linea) == HIGH)
  {
    sim.handleInput(pkt);
    layer4.incapsulaDati(pkt);
    layer3.incapsulaDati(pkt);
    layer2.incapsulaDati(pkt);
    layer1.incapsulaDati(pkt);
    layer1.inviaPacchetto(pkt);

  }
}