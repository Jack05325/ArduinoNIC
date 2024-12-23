#include <Arduino.h>
#include "CRC16.h"
#include "CRC.h"
#include "CrcFastReverse.h"

CRC16 crc;

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
uint8_t frame[4];
const size_t frameLength = sizeof(frame);
uint8_t frameRX[4];

void Rx();
void Tx();

void setup()
{
  pinMode(linea, INPUT);
  pinMode(pullUP, OUTPUT);
  Serial.begin(9600);
  pinMode(butt, INPUT_PULLUP);
  digitalWrite(linea, HIGH);
}

void loop()
{
  while (digitalRead(butt) == HIGH)
  {
    if (digitalRead(linea) == LOW)
    {
      //Rx();
    }
  }
  if (digitalRead(linea) == HIGH)
  {
    Tx();
  }
}

void Rx()
{
  delayMicroseconds(Tbit + (Tbit / 2));

  for (int i = 0; i < frameLength; i++)
  {
    for(int j = 0; j < 8; j++){
      bitWrite(frameRX[i], j, digitalRead(linea));
      delayMicroseconds(Tbit);
    }
    
  }

  crc.reset();
  crc.add(frameRX[0]);
  crc.add(frameRX[1]);
  uint16_t crcVal = crc.calc();
  uint16_t crcRX = frame[2] | (frameRX[3]<<8);
  if(crcVal == crcRX){
    idDestRX = frameRX[0] >> 4;
    idMitt   = frameRX[0] & 0xF;
    dato     = frameRX[1];

    if (idDestRX == id || idDest == idBroadcast)
    {
      Serial.print("Nuovo messaggio da ");
      Serial.print(idMitt);
      Serial.print(" il messaggio è ");
      Serial.println(dato);
    }
    else
    {
      Serial.println("Nuovo messaggio non per me");
    }
  }else{
    Serial.println("Messaggio ricevuto errato, CRC diverso");
  }

  
}

void Tx()
{
  Serial.println("Invio");

  frame[0] = ((unsigned long)idDest << 4) | id;
  frame[1] = dato;
  crc.reset();
  crc.add(frame[0]);
  crc.add(frame[1]);
  uint16_t crcVal = crc.calc();
  frame[2] = (crcVal << 8) >> 8;
  frame[3] = crcVal >> 8;

  digitalWrite(linea, start);
  delayMicroseconds(Tbit);
  for (int i = 0; i < frameLength; i++)
  {
    for(int j = 0; j < 8; j++){
      bool bit = bitRead(frame[i], j);
      digitalWrite(linea, bit);
      delayMicroseconds(Tbit);
      Serial.print(bit);
    }
  }
  digitalWrite(linea, stop);
  delayMicroseconds(Tbit);

  delay(400);
}