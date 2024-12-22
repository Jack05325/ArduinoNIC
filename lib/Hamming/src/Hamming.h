#ifndef HAMMING_H
#define HAMMING_H

#include <Arduino.h>

class Hamming {
  public:
    Hamming();
    long long calcNewFrame(long long frame);
    long long checkFrameRicevuto(long long frame);
    int getLunghezzaData(long data);
    long long estraiDati(long long frame);
  private:
    int calcNbitControllo(long long frame);
    int* calcPosizioniBitControllo(long long frame);
    int* calcValoreBitControllo(long long frame);
};

#endif