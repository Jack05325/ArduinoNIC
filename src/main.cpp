#include <Arduino.h>
#include <Hamming.h>

Hamming hamming;

const int pullUP = 7;
const int linea = 8;
const int butt = 4;

const int start = 0;
const int stop = 1;
const int Tbit = 100;

const int id = 5; // ID del mittente
char dato = 'a';  // Dato da trasmettere
const int idDest = 3; // ID del destinatario
const int idBroadcast = 15; // ID broadcast per tutti i nodi

int idDestRX = 3; // ID destinatario ricevuto
int idMitt;       // ID mittente ricevuto

uint8_t frame[2]; // Frame da trasmettere
int frameLength = 2;           // Lunghezza del frame in byte

void Rx();
void Tx();

void setup() {
  pinMode(linea, INPUT);
  pinMode(pullUP, OUTPUT);
  pinMode(butt, INPUT_PULLUP);

  Serial.begin(9600);
  digitalWrite(linea, HIGH);
}

void loop() {
  if (digitalRead(butt) == HIGH) {
    if (digitalRead(linea) == LOW) {
      Rx();
    }
  } else if (digitalRead(linea) == HIGH) {
    Tx();
  }
}

void Rx() {
  delayMicroseconds(Tbit + Tbit / 2);

  for (int i = 0; i < frameLength; i++) {
    frame[i] = 0;
    for (int j = 0; j < 8; j++) {
      bitWrite(frame[i], j, digitalRead(linea));
      delayMicroseconds(Tbit);
    }
  }

  uint8_t* checkedFrame = hamming.checkFrameRicevuto(frame, frameLength);
  uint8_t* data = hamming.estraiDati(checkedFrame, frameLength);

  // Estrazione di ID destinatario, ID mittente e dato
  idDestRX = (data[1] >> 4) & 0xF;
  idMitt = data[1] & 0xF;
  dato = data[0];

  Serial.println("Frame ricevuto corretto:");
  for (int i = 0; i < frameLength; i++) {
    for (int j = 0; j < 8; j++) {
      Serial.print((bool)bitRead(checkedFrame[i], j));
    }
  }
  Serial.println();

  if (idDestRX == id || idDestRX == idBroadcast) {
    Serial.print("Nuovo messaggio da ");
    Serial.print(idMitt);
    Serial.print(": ");
    Serial.println(dato);
  } else {
    Serial.println("Nuovo messaggio non per me");
  }

  delete[] checkedFrame;
  delete[] data;
}

void Tx() {
  Serial.println("Preparazione per inviare il messaggio...");

  // Creazione del frame base
  frame[1] = (idDest << 4) | id;
  frame[0] = dato;

  Serial.println("Frame originale (senza Hamming):");
  for (int i = frameLength - 1; i >= 0; i--) {
    for (int j = 7; j >= 0; j--) {
      Serial.print((bool)bitRead(frame[i], j));
    }
  }
  Serial.println();

  uint8_t* newFrame = hamming.calcNewFrame(frame, frameLength);

  Serial.println("Frame con Hamming:");
  for (int i = frameLength + 1; i >= 0; i--) { // Lunghezza corretta
    for (int j = 7; j >= 0; j--) {
      Serial.print((bool)bitRead(newFrame[i], j));
    }
  }
  Serial.println();

  // Simulazione della trasmissione
  digitalWrite(linea, start);
  delayMicroseconds(Tbit);
  for (int i = 0; i < frameLength; i++) {
    for (int j = 0; j < 8; j++) {
      bool bit = bitRead(newFrame[i], j);
      digitalWrite(linea, bit);
      delayMicroseconds(Tbit);
    }
  }
  digitalWrite(linea, stop);
  delayMicroseconds(Tbit);

  delete[] newFrame;

  Serial.println("Messaggio trasmesso con successo.");
  delay(400);
}