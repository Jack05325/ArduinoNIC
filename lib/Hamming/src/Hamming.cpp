#include "Hamming.h"

Hamming::Hamming() {
    // Costruttore, attualmente vuoto
}

int Hamming::getLunghezzaData(long long data) {
    int lunghezza = 0;
    while (data > 0) {
        data = data >> 1;
        lunghezza++;
    }
    return lunghezza;
}

int Hamming::calcNbitControllo(int frameLength) {
    int i = 1;
    // Calcoliamo il numero di bit di controllo necessari
    while ((1 << i) < (frameLength * 8 + i + 1)) {
        i++;
    }
    return i;
}

int* Hamming::calcPosizioniBitControllo(int nBitControllo) {
    // Calcoliamo le posizioni dei bit di controllo
    int* posizioni = new int[nBitControllo];
    for (int i = 0; i < nBitControllo; i++) {
        posizioni[i] = (1 << i);  // Le posizioni sono 1, 2, 4, 8, ...
    }
    return posizioni;
}

int* Hamming::calcValoreBitControllo(uint8_t* frame, int frameLength, int nBitControllo) {
    // Calcoliamo i valori dei bit di controllo
    int* valori = new int[nBitControllo];
    int* pos = calcPosizioniBitControllo(nBitControllo);
    for (int i = 0; i < nBitControllo; i++) {
        valori[i] = 0;
        for (int j = pos[i] - 1; j < frameLength * 8 + nBitControllo; j++) {
            // Ogni bit di controllo calcola i suoi valori tramite XOR
            if ((j + 1) & pos[i]) {
                int byteIndex = j / 8;
                int bitIndex = j % 8;
                valori[i] ^= bitRead(frame[byteIndex], bitIndex);
            }
        }
    }
    delete[] pos;
    return valori;
}

uint8_t* Hamming::calcNewFrame(uint8_t* frame, int frameLength) {
    // Calcoliamo un nuovo frame con bit di controllo
    int nBitControllo = calcNbitControllo(frameLength);
    int totalLength = frameLength * 8 + nBitControllo; // Numero totale di bit nel frame
    uint8_t* newFrame = new uint8_t[(totalLength + 7) / 8]; // Allocazione nuova memoria

    int* posizioniBitControllo = calcPosizioniBitControllo(nBitControllo);
    int* valoriBitControllo = calcValoreBitControllo(frame, frameLength, nBitControllo);
    int dataPos = 0;
    int bitControlloPosizionati = 0;

    for (int i = 0; i < totalLength; i++) {
        int byteIndex = i / 8;
        int bitIndex = i % 8;

        // Se siamo nella posizione di un bit di controllo, inseriamo il bit di controllo
        if (i + 1 == posizioniBitControllo[bitControlloPosizionati]) {
            bitWrite(newFrame[byteIndex], bitIndex, valoriBitControllo[bitControlloPosizionati]);
            bitControlloPosizionati++;
        }
        // Altrimenti, copiamo i dati nel nuovo frame
        else {
            if (dataPos < frameLength * 8) {
                bitWrite(newFrame[byteIndex], bitIndex, bitRead(frame[dataPos / 8], dataPos % 8));
                dataPos++;
            }
        }
    }

    delete[] posizioniBitControllo;
    delete[] valoriBitControllo;

    return newFrame;
}

uint8_t* Hamming::checkFrameRicevuto(uint8_t* frame, int frameLength) {
    int nBitControllo = calcNbitControllo(frameLength);
    int* pos = calcPosizioniBitControllo(nBitControllo);
    int* valori = calcValoreBitControllo(frame, frameLength, nBitControllo);

    // Calcoliamo la posizione dell'errore
    int errorePos = 0;
    for (int i = 0; i < nBitControllo; i++) {
        errorePos += valori[i] * pos[i];
    }
    errorePos--;

    // Se c'è un errore, correggiamo il frame
    if (errorePos >= 0) {
        int byteIndex = errorePos / 8;
        int bitIndex = errorePos % 8;
        bitWrite(frame[byteIndex], bitIndex, !bitRead(frame[byteIndex], bitIndex));
    }

    delete[] pos;
    delete[] valori;
    
    return frame;
}

uint8_t* Hamming::estraiDati(uint8_t* frame, int frameLength) {
    int nBitControllo = calcNbitControllo(frameLength);
    int* pos = calcPosizioniBitControllo(nBitControllo);

    // Estrazione dei dati, escludendo i bit di controllo
    uint8_t* dati = new uint8_t[frameLength - nBitControllo];
    int dataPos = 0;
    int framePos = 0;
    for (int i = 0; i < (frameLength * 8); i++) {
        int byteIndex = i / 8;
        int bitIndex = i % 8;
        bool isBitControllo = false;

        for (int j = 0; j < nBitControllo; j++) {
            if (i + 1 == pos[j]) {
                isBitControllo = true;
                break;
            }
        }

        if (!isBitControllo) {
            bitWrite(dati[dataPos / 8], dataPos % 8, bitRead(frame[byteIndex], bitIndex));
            dataPos++;
        }
    }

    delete[] pos;
    return dati;
}