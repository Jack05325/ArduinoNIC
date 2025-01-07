import socket

def make_socket_request(host, port):
    try:
        with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
            s.connect((host, port))
            request = f"GET / HTTP/1.1\r\nHost: {host}\r\n\r\n"
            s.sendall(request.encode())
            response = s.recv(4096)
            return response.decode()
    except socket.error as e:
        print(f"An error occurred: {e}")
        return None
if __name__ == "__main__":
    host = "example.com"
    port = 80
    response = make_socket_request(host, port)
    if response:
        print(response)