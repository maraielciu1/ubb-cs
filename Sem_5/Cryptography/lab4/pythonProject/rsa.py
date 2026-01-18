# import math
#
#
# class RSA_Base27:
#     def __init__(self, p, q, k, l):
#         self.p = p
#         self.q = q
#         self.k = k  # Input block size
#         self.l = l  # Output block size
#         self.n = p * q
#         self.phi = (p - 1) * (q - 1)
#         self.e = self._find_e()
#
#         # Validation to ensure math works
#         max_input_val = (27 ** k) - 1
#         if max_input_val >= self.n:
#             print(f"WARNING: Input block size k={k} is too large for n={self.n}.")
#             print("Message information may be lost (b >= n).")
#
#     def _is_prime(self, num):
#         if num < 2: return False
#         for i in range(2, int(math.sqrt(num)) + 1):
#             if num % i == 0: return False
#         return True
#
#     def _find_e(self):
#         # Start searching for e at the smallest odd prime (3)
#         candidate = 3
#         while True:
#             if self._is_prime(candidate):
#                 if math.gcd(candidate, self.phi) == 1:
#                     return candidate
#             candidate += 2  # Check next odd number
#
#     def _char_to_val(self, char):
#         # Map characters to Base-27 values
#         if char == ' ':
#             return 0
#         if 'A' <= char.upper() <= 'Z':
#             return ord(char.upper()) - 64
#         raise ValueError(f"Character '{char}' not in alphabet (Space + A-Z)")
#
#     def _val_to_char(self, val):
#         # Map values back to characters
#         if val == 0:
#             return ' '
#         return chr(val + 64)
#
#     def _text_to_int_block(self, text_segment):
#         # Convert string segment to integer b
#         # Formula: val(char) * 27^(position power)
#         total = 0
#         length = len(text_segment)
#         for i, char in enumerate(text_segment):
#             power = length - 1 - i
#             val = self._char_to_val(char)
#             total += val * (27 ** power)
#         return total
#
#     def _int_to_text_block(self, number, block_len):
#         # Convert integer c back to string of length l
#         chars = []
#         for i in range(block_len):
#             power = block_len - 1 - i
#             divisor = 27 ** power
#
#             digit_val = number // divisor
#             number = number % divisor
#
#             chars.append(self._val_to_char(digit_val))
#         return "".join(chars)
#
#     def encrypt(self, message):
#         print(f"--- Encryption Parameters ---")
#         print(f"n = {self.n}, phi = {self.phi}, e = {self.e}")
#         print(f"Message: '{message}'")
#
#         encrypted_string = ""
#
#         # 1. Padding
#         while len(message) % self.k != 0:
#             message += " "  # Pad with Space (0)
#
#         # 2. Process Blocks
#         for i in range(0, len(message), self.k):
#             # Get block of k letters
#             block_text = message[i: i + self.k]
#
#             # Convert to numerical b
#             b = self._text_to_int_block(block_text)
#
#             # Encrypt: c = b^e mod n
#             c = pow(b, self.e, self.n)
#
#             # Convert to output block of l letters
#             cipher_block = self._int_to_text_block(c, self.l)
#
#             print(f"Block '{block_text}' -> Int {b} -> Encrypted {c} -> Cipher '{cipher_block}'")
#             encrypted_string += cipher_block
#
#         return encrypted_string
#
#
# # ==========================================
# # TEST CASE (Matches the Example in Chat)
# # ==========================================
#
# # Parameters
# p = 31
# q = 53
# k = 2
# l = 3
#
# rsa = RSA_Base27(p, q, k, l)
# final_cipher = rsa.encrypt("ALGEBRA ")
#
# print(f"\nFINAL OUTPUT: {final_cipher}")

import math


class RSA_Base27:
    def __init__(self, p, q, k, l):
        self.p = p
        self.q = q
        self.k = k  # Input block size
        self.l = l  # Output block size
        self.n = p * q
        self.phi = (p - 1) * (q - 1)
        self.e = self._find_e()

        # Validation to ensure math works
        max_input_val = (27 ** k) - 1
        if max_input_val >= self.n:
            print(f"WARNING: Input block size k={k} is too large for n={self.n}.")
            print("Message information may be lost (b >= n).")

    def _is_prime(self, num):
        if num < 2: return False
        for i in range(2, int(math.sqrt(num)) + 1):
            if num % i == 0: return False
        return True

    def _find_e(self):
        # Start searching for e at the smallest odd prime (3)
        candidate = 3
        while True:
            if self._is_prime(candidate):
                if math.gcd(candidate, self.phi) == 1:
                    return candidate
            candidate += 2  # Check next odd number

    def _char_to_val(self, char):
        # Map characters to Base-27 values
        if char == ' ':
            return 0
        if 'A' <= char.upper() <= 'Z':
            return ord(char.upper()) - 64
        raise ValueError(f"Character '{char}' not in alphabet (Space + A-Z)")

    def _val_to_char(self, val):
        # Map values back to characters
        if val == 0:
            return ' '
        return chr(val + 64)

    def _text_to_int_block(self, text_segment):
        # Convert string segment to integer b
        # Formula: val(char) * 27^(position power)
        total = 0
        length = len(text_segment)
        for i, char in enumerate(text_segment):
            power = length - 1 - i
            val = self._char_to_val(char)
            total += val * (27 ** power)
        return total

    def _int_to_text_block(self, number, block_len):
        # Convert integer c back to string of length l
        chars = []
        for i in range(block_len):
            power = block_len - 1 - i
            divisor = 27 ** power

            digit_val = number // divisor
            number = number % divisor

            chars.append(self._val_to_char(digit_val))
        return "".join(chars)

    def encrypt(self, message):
        print(f"--- Encryption Parameters ---")
        print(f"n = {self.n}, phi = {self.phi}, e = {self.e}")
        print(f"Message: '{message}'")

        encrypted_string = ""

        # 1. Padding
        while len(message) % self.k != 0:
            message += " "  # Pad with Space (0)

        # 2. Process Blocks
        for i in range(0, len(message), self.k):
            # Get block of k letters
            block_text = message[i: i + self.k]

            # Convert to numerical b
            b = self._text_to_int_block(block_text)

            # Encrypt: c = b^e mod n
            c = pow(b, self.e, self.n)

            # Convert to output block of l letters
            cipher_block = self._int_to_text_block(c, self.l)

            print(f"Block '{block_text}' -> Int {b} -> Encrypted {c} -> Cipher '{cipher_block}'")
            encrypted_string += cipher_block

        return encrypted_string

    def decrypt(self, ciphertext):
        print(f"\n--- Decryption Parameters ---")
        print(f"n = {self.n}, phi = {self.phi}, e = {self.e}")

        # Compute d, the modular multiplicative inverse of e mod phi
        d = pow(self.e, -1, self.phi)
        print(f"d = {d}")

        plaintext = ""

        # Process each cipher block of length l
        for i in range(0, len(ciphertext), self.l):
            cipher_block = ciphertext[i: i + self.l]

            # Convert block to integer c
            c = self._text_to_int_block(cipher_block)

            # Decrypt: b = c^d mod n
            b = pow(c, d, self.n)

            # Convert integer b back to text of length k
            plain_block = self._int_to_text_block(b, self.k)

            print(f"Cipher block '{cipher_block}' -> Int {c} -> Decrypted {b} -> Text '{plain_block}'")

            plaintext += plain_block

        return plaintext.rstrip()  # remove padding spaces



# Parameters
p = 43
q = 89
k = 2
l = 3

rsa = RSA_Base27(p, q, k, l)
final_cipher = rsa.encrypt("VIENNA")
final_decipher = rsa.decrypt(" FKBGM AN")

print(f"\nFINAL OUTPUT: (encrypted: '{final_cipher}') (decrypted: '{final_decipher}')")