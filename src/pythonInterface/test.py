import math

def find_gcd(a, b):
    while b:
        a, b = b, a % b
    return a

def extended_euclidean_algorithm(a, b):
    if a == 0:
        return b, 0, 1
    else:
        gcd, x, y = extended_euclidean_algorithm(b, a % b)
        return gcd, y - (a // b) * x, x

def get_inverse_modulo(a, m):
    gcd, x, _ = extended_euclidean_algorithm(a, m)
    if gcd != 1:
        return None
    else:
        return x % m

def rsa_decrypt(cipher_text, d, n):
    plain_text = ''
    for char in cipher_text.split():
        encrypted_char = int(char)
        decrypted_char = pow(encrypted_char, d, n)
        plain_text += chr(decrypted_char)
    return plain_text

# Example usage
d = get_inverse_modulo(7, 77)  # private exponent (d)
cipher_text = "1234567891011"  # encrypted text
plain_text = rsa_decrypt(cipher_text, d, N)
print("Cipher Text:", cipher_text)
print("Plain Text:", plain_text)