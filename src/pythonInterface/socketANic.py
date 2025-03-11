import serial
import time
import random
from typing import List
import serial.tools
import serial.tools.list_ports
from UDP import udp

class socket:
    _UDP = udp()
    def __init__(self, addr: str, port: str, interface = None):
        self._addr = addr
        self._port = port
        self._serial_interface = interface
        if(interface != None):
            self._set_nic_param(self._addr, self._port);
    
    def _set_nic_param(self, ip, port):
        #self._send_data_on_serial_interface(self._parse_text_for_serial([ip, port]))
        pass

    def set_serial_interface(self, interface: serial):
        self._serial_interface = interface
        self._set_nic_param(self._addr, self._port)

#    def sendUdp(self, ip_destinazione, porta_destinazione, data):
#        self._parse_text_for_serial([ip_destinazione, 128, porta_destinazione, self._port, data])
#    
#        return 0
    def sendData(self, destination_addr, destination_port, data, protocol = _UDP):
        assert destination_port != None or destination_port != None or data != None, "Error, the destination addr/port and the data variable can not be empty"
        data_to_send = protocol.encaplateData(self._addr, destination_addr, self._port, destination_port, data)
        #data_from_serial = self._send_data_on_serial_interface(data_to_send)
        #return data_from_serial
        

    def _send_data_on_serial_interface(self, data : str):
            self._serial_interface.reset_input_buffer()
            self._serial_interface.write(bytes(data , 'utf-8'))
            data = self._serial_interface.readline()
            return data

 


