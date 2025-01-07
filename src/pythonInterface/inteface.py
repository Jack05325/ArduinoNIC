import serial
import time
import random
import serial.tools
import serial.tools.list_ports
from Parametri import *
from Flags import *



# Configurazione porta seriale
arduino = serial.Serial('/dev/cu.usbserial-120', baudrate=115200, timeout=2)

def write_read(x):
    arduino.reset_input_buffer()  # Pulisci il buffer
    arduino.write(bytes(x , 'utf-8'))  # Aggiungi terminatore
    time.sleep(0.1)  # Tempo di attesa per la risposta
    data = arduino.readlines()
    return data

while True:
    IP_DESTINAZIONE = input("inserisci l'ip di destinazione (192.168.1.1): ")
    TTL = int(input("inserisci il TTL: "))
    PORTA_DESTINAZIONE = int(input("inserisci la porta di destinazione: "))
    PORTA_MITTENTE = random.randint(0, 2**16-1)
    DATI = input("inserisci i dati: ")
    ricevuto = write_read(f"{IP_DESTINAZIONE}|{TTL}|{PORTA_DESTINAZIONE}|{PORTA_MITTENTE}|{DATI}")
    print(ricevuto)

    
    