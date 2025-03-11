import serial
import time
import random
import serial.tools
import serial.tools.list_ports
from socketANic import socket
from Parametri import *
from Flags import *



socket = socket("192.168.0.1", "8080", serial.Serial('/dev/cu.debug-console', baudrate=115200, timeout=2))

while True:
    IP_DESTINAZIONE = input("inserisci l'ip di destinazione (Es: 192.168.1.1): ")
    #TTL = int(input("inserisci il TTL: "))
    PORTA_DESTINAZIONE = input("inserisci la porta di destinazione: ")
    #PORTA_MITTENTE = random.randint(0, 2**16-1)
    DATI = input("inserisci i dati: ")
    
    ricevuto = socket.sendData(IP_DESTINAZIONE, PORTA_DESTINAZIONE, DATI)
    print(ricevuto)

    
    