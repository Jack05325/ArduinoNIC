#include "Hamming.h"
#include "Array.h"

Hamming::Hamming()
{
}

int Hamming::getLunghezzaData(long data)
{
    int lunghezza = 0;
    while (data > 0)
    {
        data = data >> 1;
        lunghezza++;
    }
    return lunghezza;
}

int Hamming::calcNbitControllo(long long frame)
{
    int i = 1;
    while ((1 << i) < (getLunghezzaData(frame) + i + 1))
    {
        i++;
    }
    return i;
}

int *Hamming::calcValoreBitControllo(long long frame)
{

    int nBitControllo = calcNbitControllo(frame);
    int *valori = new int[nBitControllo];
    int *pos = calcPosizioniBitControllo(frame);
    //Serial.print("l frame: ");
    //Serial.println(getLunghezzaData(frame));
    //Serial.println("Posizioni: ");
    for (int i = 0; i < nBitControllo; i++)
    {
        valori[i] = 0;
        for (int j = pos[i]; j < getLunghezzaData(frame) + nBitControllo + 1; j++)
        {
            if ((j & (pos[i])) != 0)
            {
                //Serial.print(j);
                valori[i] ^= bitRead(frame, j - 1);
            }
        }
        //Serial.println();
    }
    //Serial.println("Valori: ");
    for (int i = 0; i < nBitControllo; i++)
    {
        //Serial.print(valori[i]);
    }
    return valori;
}

int *Hamming::calcPosizioniBitControllo(long long frame)
{
    int nBitControllo = calcNbitControllo(frame);
    int *posizioni = new int[nBitControllo];

    for (int i = 0; i < nBitControllo; i++)
    {
        posizioni[i] = (1 << i);
        //Serial.println(posizioni[i]);
    }
    return posizioni;
}

long long Hamming::calcNewFrame(long long frame)
{
    int dataLength = getLunghezzaData(frame);     // Lunghezza del frame originale
    int nBitControllo = calcNbitControllo(frame); // Numero di bit di controllo
    long long newFrame = 0LL;
    int totalLength = (dataLength + nBitControllo); // Lunghezza totale del nuovo frame
    int bitControlloPosizionati = 0;
    int dataPos = 0;
    int *posizioneBitControllo = calcPosizioniBitControllo(frame);
    int *valoreBitControllo = calcValoreBitControllo(frame);

    for (int i = 0; i < totalLength; i++)
    {
        if (i == posizioneBitControllo[bitControlloPosizionati] - 1)
        {
            bitWrite(newFrame, i, valoreBitControllo[bitControlloPosizionati]);
            bitControlloPosizionati++;
        }
        else
        {
            bitWrite(newFrame, i, bitRead(frame, dataPos));
            dataPos++;
        }
    }

    return newFrame;
}

long long Hamming::checkFrameRicevuto(long long frame) {

    int nBitControllo = calcNbitControllo(frame);
    int frameLength = getLunghezzaData(frame) + nBitControllo;

    int* pos = calcPosizioniBitControllo(frame);

    int* valori = calcValoreBitControllo(frame);
    int valoreErrato = 0;

    for (int i = 0; i < nBitControllo; i++) {
        valoreErrato += valori[i] * pos[i];
    }

    if (valoreErrato == 0) {
        
    } else {
        bitWrite(frame, valoreErrato - 1, !bitRead(frame, valoreErrato - 1));

    }

    delete[] pos;
    delete[] valori;

    return frame;
}

long long Hamming::estraiDati(long long frame){
    int nBitControllo = calcNbitControllo(frame);
    int frameLength = getLunghezzaData(frame) + nBitControllo;

    int* pos = calcPosizioniBitControllo(frame);

    long long dati = 0LL;
    int dataPos = 0; 

    for (int i = 1; i <= frameLength; i++) {
        bool isBitDiControllo = false;
        for (int j = 0; j < nBitControllo; j++) {
            if (i == pos[j]) {
                isBitDiControllo = true;
                break;
            }
        }

        if (!isBitDiControllo) {
            bitWrite(dati, dataPos, bitRead(frame, i - 1));
            dataPos++;
        }
    }

    delete[] pos;

    return dati;
}