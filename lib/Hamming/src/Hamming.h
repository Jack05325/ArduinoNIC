#ifndef HAMMING_H
#define HAMMING_H

#include <Arduino.h>

class Hamming {
  public:
    Hamming();
    
    // Calcola il nuovo frame con bit di controllo
    uint8_t* calcNewFrame(uint8_t* frame, int frameLength);
    
    // Controlla un frame ricevuto e corregge eventuali errori
    uint8_t* checkFrameRicevuto(uint8_t* frame, int frameLength);
    
    // Ottiene la lunghezza in bit di un dato
    int getLunghezzaData(long long data);
    
    // Estrae i dati da un frame con bit di controllo
    uint8_t* estraiDati(uint8_t* frame, int frameLength);

  private:
    // Calcola il numero di bit di controllo necessari
    int calcNbitControllo(int frameLength);

    // Calcola le posizioni dei bit di controllo
    int* calcPosizioniBitControllo(int nBitControllo);

    // Calcola i valori dei bit di controllo
    int* calcValoreBitControllo(uint8_t* frame, int frameLength, int nBitControllo);
};

#endif