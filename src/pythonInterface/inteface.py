import serial
import time

arduino = serial.Serial('/dev/cu.usbserial-1210', baudrate=115200, timeout=.1)

flags = {
	"SYN" : 1,
	"ACK" : 2,
}

dataType = 00

def write_read(x): 
	arduino.write(bytes(x, 'utf-8')) 
	time.sleep(.1) 
	data = arduino.readlines()
	return data 

while True: 
	data = input("Inserisci i dati: ") 
	l4Protocol = input("protocollo l4: ")	
	dataType = input("tipo di dato: ")
	pkt = write_read(str(flags[dataType]) + data + l4Protocol) 
	print(pkt) 
