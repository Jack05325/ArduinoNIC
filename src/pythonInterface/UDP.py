from typing import List

class udp():
    def __init__(self):
        self._MAX_PAYLOAD_SIZE = 16
    
    def encaplateData(self, source_addr, destination_addr, source_port, destination_port, payload):
        return self._parse_text_for_serial([source_addr, destination_addr, 128, destination_port, source_port, payload]) 

    def _parse_text_for_serial (self, encaplusation_data: List[str]):
        assert len(encaplusation_data) != 0, "Error: the list of data can not be empty"
        parse_header = ""
        for i in encaplusation_data[0:-1]: 
            parse_header += str(i) + "|"

        packets = [ parse_header + ( encaplusation_data[-1][i : i + self._MAX_PAYLOAD_SIZE]) for i in range(0, len(encaplusation_data[-1]), self._MAX_PAYLOAD_SIZE)]
        for i, j in zip(packets, range(len(packets))):
            print(j, i)
        total_char = 0
        for i in packets:
            total_char += len(i)
        print(total_char)
        return packets