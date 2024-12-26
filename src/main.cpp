#include <Arduino.h>
#include "Layer4.h"
#include "CRC16.h"
#include "CRC.h"

CRC16 crc;
Layer4 layer4;
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
String dati;

struct pacchetto
{
  uint8_t dati[16];
  uint8_t layer4[17];
  uint8_t layer3[11];
  uint8_t layer2[7];
  uint8_t layer1[2];
};
struct pacchetto pkt;
int dataType;
uint8_t frameRX[sizeof(pacchetto)];
//void Rx();
//void Tx();

void setup()
{
  pinMode(linea, INPUT);
  pinMode(pullUP, OUTPUT);
  Serial.begin(115200);
  pinMode(butt, INPUT_PULLUP);
  digitalWrite(linea, HIGH);
}

void loop()
{
  while (!(Serial.available() > 0))
  {
    if (digitalRead(linea) == LOW)
    {
      //Rx();
    }
  }
  if (digitalRead(linea) == HIGH)
  {
    dati = Serial.readString();    
    Serial.println(dati);
    dataType = dati.substring(0, 3).toInt();
    switch (dataType)
    {
      case 00:
        Serial.println("Dati");
        break;
      case 01:
        Serial.println("ACK");
        layer4.setFlags(0x01);
        break;
      case 10:
        Serial.println("SYN");
        layer4.setFlags(0x02);
        break;
      default:
        break;
    }
    dati.getBytes(pkt.dati, 16);
    Serial.println((char*)pkt.dati);
    layer4.incapsulaDati(pkt);
    Serial.println("Layer4: ");
    for(int i = 0; i < sizeof(pkt.layer4); i++){
      Serial.print(pkt.layer4[i], HEX);
      Serial.print(" ");
    }
    //Tx();
  }
}

/*void Rx()
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
}*/