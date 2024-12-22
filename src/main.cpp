#include <Arduino.h>
#include <Hamming.h>

Hamming hamming;

int pullUP = 7;
int linea = 8;
int start = 0;
int stop = 1;
int Tbit = 100;

int id = 5;      // "IP" del mittente
char dato = 'a'; // TX
int idDest = 3;

int idDestRX = 3; // RX
int idMitt;
int idBroadcast = 15;
std::bitset<16> frame;
bitset<size_t maxBits> frame;

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
      Rx();
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

  for (int i = 0; i < 16; i++)
  {
    bitWrite(frame, i, digitalRead(linea));
    delayMicroseconds(Tbit);
  }
  frame = hamming.checkFrameRicevuto(frame);
  frame = hamming.estraiDati(frame);

  idDestRX = (unsigned int)frame >> 12;
  idMitt = (unsigned int)(frame << 4) >> 12;
  dato = (unsigned int)(frame << 8) >> 8;

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
}

void Tx()
{
  Serial.println("Invio");
  frame = 0LL;

  frame = frame | ((unsigned long)idDest << 12);
  frame = frame | ((unsigned long)id << 8);
  frame = frame | dato;

  unsigned long long frameNew = hamming.calcNewFrame(frame);


  digitalWrite(linea, start);
  delayMicroseconds(Tbit);
  int frameNewSize = hamming.getLunghezzaData(frameNew);
  Serial.println(frameNewSize);
  for (int i = 0; i < frameNewSize; i++)
  {
    bool bit = bitRead(frameNew, i);
    digitalWrite(linea, bit);
    delayMicroseconds(Tbit);
    //Serial.print(bit);
  }

  digitalWrite(linea, stop);
  delayMicroseconds(Tbit);

  delay(400);
}