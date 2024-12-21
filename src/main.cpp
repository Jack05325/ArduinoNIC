#include <Arduino.h>

int pullUP = 7;
int linea = 8;
int start = 0;
int stop = 1;
int Tbit = 100;

int id = 5; // "IP" del mittente
char dato = 'a'; // TX
int idDest = 3;

int idDestRX = 3; // RX
int idMitt;
int idBroadcast = 15;
int butt = 4;
unsigned long long frame;

void Rx();
void Tx();
int getLunghezzaData(long data);
int calcNbitControllo(long long frame);
int* calcPosizioniBitControllo(long long frame);
long long calcNewFrame(long long frame);
int* calcValoreBitControllo(long long frame);

void setup() {
  pinMode(linea, INPUT);
  pinMode(pullUP, OUTPUT);
  Serial.begin(9600);
  pinMode(butt, INPUT_PULLUP);
  digitalWrite(linea, HIGH);
}

void loop() {
  while (digitalRead(butt) == HIGH) {
    if (digitalRead(linea) == LOW) {
      Rx();
    }
  }
  if (digitalRead(linea) == HIGH) {
    Tx();
  }
}

int getLunghezzaData(long data) {
  int lunghezza = 0;
  while (data > 0) {
    data = data >> 1;
    lunghezza++;
  }
  return lunghezza;
}

int calcNbitControllo(long long frame) {
  int i = 1;
  while ((1 << i) < (getLunghezzaData(frame) + i + 1)) {
    i++;
  }
  return i;
}

int* calcValoreBitControllo(long long frame){
  int nBitControllo = calcNbitControllo(frame);
  int* valori = new int[nBitControllo];
  int* pos = calcPosizioniBitControllo(frame);
  Serial.print("l frame: ");
  Serial.println(getLunghezzaData(frame));
  Serial.println("Posizioni: ");
  for (int i = 0; i < nBitControllo; i++){
    valori[i] = 0;
    for (int j = pos[i]; j < getLunghezzaData(frame) + nBitControllo + 1; j++){      
      if ((j & (pos[i])) != 0){
        Serial.print(j);
        valori[i] ^= bitRead(frame, j-1);
      }
    }
    Serial.println();
  }
  Serial.println("Valori: ");
  for(int i = 0; i < nBitControllo; i++){
    Serial.print(valori[i]);
  }
  return valori;
}

long long calcNewFrame(long long  frame) {
  int dataLength = getLunghezzaData(frame); // Lunghezza del frame originale
  int nBitControllo = calcNbitControllo(frame); // Numero di bit di controllo
  long long newFrame = 0LL;
  int totalLength = (dataLength + nBitControllo); // Lunghezza totale del nuovo frame
  int bitControlloPosizionati = 0;
  int dataPos = 0;
  int* posizioneBitControllo = calcPosizioniBitControllo(frame);
  int* valoreBitControllo = calcValoreBitControllo(frame);

  for(int i = 0; i < totalLength; i++) {
    if(i == posizioneBitControllo[bitControlloPosizionati]-1) {
      bitWrite(newFrame, i, valoreBitControllo[bitControlloPosizionati]);
      bitControlloPosizionati++;
    } else {
      bitWrite(newFrame, i, bitRead(frame, dataPos));
      dataPos++;
    }
  }

  return newFrame;
}

int* calcPosizioniBitControllo(long long frame) {
  int nBitControllo = calcNbitControllo(frame);
  int* posizioni = new int[nBitControllo];

  for (int i = 0; i < nBitControllo; i++) {
    posizioni[i] = (1 << i);
    Serial.println(posizioni[i]);
  }
  return posizioni;
}


void Rx() {
  delayMicroseconds(Tbit + (Tbit / 2));

  for (int i = 0; i < 16; i++) {
    bitWrite(frame, i, digitalRead(linea));
    delayMicroseconds(Tbit);
  }

  idDestRX = (unsigned int)frame >> 12;
  idMitt = (unsigned int)(frame << 4) >> 12;
  dato = (unsigned int)(frame << 8) >> 8;

  if (idDestRX == id || idDest == idBroadcast) {
    Serial.print("Nuovo messaggio da ");
    Serial.print(idMitt);
    Serial.print(" il messaggio è ");
    Serial.println(dato);
  } else {
    Serial.println("Nuovo messaggio non per me");
  }
}

void Tx() {
  Serial.println("Invio");
  frame = 0LL;

  frame = frame | ((unsigned long)idDest << 12);
  frame = frame | ((unsigned long)id << 8);
  frame = frame | dato;
  
  Serial.print("Frame originale: ");
  for (int i = (int)sizeof(frame) * 8 - 1; i >= 0; i--) {
    Serial.print((bool)bitRead(frame, i));
  }
  Serial.println();
  unsigned long long frameNew = calcNewFrame(frame);

  Serial.print("Frame con bit di controllo: ");
  for (int i = (int)sizeof(frameNew) * 8 - 1; i >= 0; i--) {
    Serial.print((bool)bitRead(frameNew, i));
  }
  Serial.println();


  digitalWrite(linea, start);
  delayMicroseconds(Tbit);
  int frameNewSize = getLunghezzaData(frameNew);
  Serial.println(frameNewSize);
  for (int i = 0; i < frameNewSize; i++) {
    bool bit = bitRead(frameNew, i);
    digitalWrite(linea, bit);
    delayMicroseconds(Tbit);
    Serial.print(bit);
  }

  digitalWrite(linea, stop);
  delayMicroseconds(Tbit);

  delay(400);
}