import random
import sys

ALPHABET = " ABCDEFGHIJKLMNOPQRSTUVWXYZ"
ALPHABET_SIZE = 27

FIXED_BIT_LENGTH = 128
FIXED_MESSAGE = "HELLO RABIN"


# we have to find x ^ 2 = c mod n
# ax + by = gcd(a,b)
def extended_gcd(a, b):
    """ Helper for Chinese Remainder Theorem. """
    if a == 0:
        return b, 0, 1
    else:
        g, y, x = extended_gcd(b % a, a)
        return g, x - (b // a) * y, y


def is_prime(n, k=40):
    """ Miller-Rabin Primality Test. """
    if n == 2 or n == 3: return True
    if n % 2 == 0 or n < 2: return False

    r, d = 0, n - 1
    while d % 2 == 0:
        r += 1
        d //= 2

    for _ in range(k):
        a = random.randrange(2, n - 1)
        x = pow(a, d, n)
        if x == 1 or x == n - 1:
            continue
        for _ in range(r - 1):
            x = pow(x, 2, n)
            if x == n - 1:
                break
        else:
            return False
    return True


def generate_large_prime(length):
    """ Generates a prime p such that p = 3 mod 4 (Blum prime). """
    while True:
        p = random.getrandbits(length)
        p |= (1 << length - 1) | 1
        if p % 4 == 3 and is_prime(p):
            return p


# ==============================================================================
# TEXT UTILITIES
# ==============================================================================

def text_to_number(text):
    m = 0
    for char in text.upper():
        if char not in ALPHABET:
            raise ValueError(f"Invalid character: '{char}'")
        m = m * ALPHABET_SIZE + ALPHABET.index(char)
    return m


def number_to_text(m):
    if m == 0: return ALPHABET[0]
    chars = []
    while m > 0:
        m, idx = divmod(m, ALPHABET_SIZE)
        chars.append(ALPHABET[idx])
    return "".join(reversed(chars))


# ==============================================================================
# MAIN LOGIC (Step-by-Step Execution)
# ==============================================================================

def main():
    print("=" * 60)
    print("RABIN CRYPTOSYSTEM - AUTOMATED DEMO")
    print("=" * 60)

    # ---------------------------------------------------------
    # STEP 1: KEY GENERATION
    # ---------------------------------------------------------
    print(f"\n[1] Generating Keys (Bit Length: {FIXED_BIT_LENGTH})...")

    p = generate_large_prime(FIXED_BIT_LENGTH)
    q = generate_large_prime(FIXED_BIT_LENGTH)
    while p == q:
        q = generate_large_prime(FIXED_BIT_LENGTH)

    n = p * q

    print(f"    -> Public Key (n): {n}")
    print(f"    -> Private Key (p): {p}")
    print(f"    -> Private Key (q): {q}")

    # ---------------------------------------------------------
    # STEP 2: ENCRYPTION
    # ---------------------------------------------------------
    print(f"\n[2] Encrypting Message: '{FIXED_MESSAGE}'")

    # Convert text to number m
    m = text_to_number(FIXED_MESSAGE)
    print(f"    -> Numeric representation (m): {m}")

    # Check capacity
    if m >= n:
        print("    [!] Error: Message too long for this key size.")
        return

    # Encrypt: c = m^2 mod n
    c = pow(m, 2, n)
    print(f"    -> Ciphertext (c): {c}")

    # ---------------------------------------------------------
    # STEP 3: DECRYPTION
    # ---------------------------------------------------------
    print(f"\n[3] Decrypting Ciphertext...")

    # Calculate roots modulo p and q (using Blum prime shortcut)'
    # find mp such that mp ^ 2 = c mod p
    mp = pow(c, (p + 1) // 4, p)
    mq = pow(c, (q + 1) // 4, q)

    # Chinese Remainder Theorem to find roots in mod n
    _, yp, yq = extended_gcd(p, q)

    r1 = (yp * p * mq + yq * q * mp) % n
    r2 = n - r1
    r3 = (yp * p * mq - yq * q * mp) % n
    r4 = n - r3

    roots = [r1, r2, r3, r4]

    print("    -> The 4 Possible Plaintexts are:")
    for i, root in enumerate(roots):
        decoded_text = number_to_text(root)

        # Highlight the correct one visually
        marker = " <--- CORRECT MESSAGE" if decoded_text == FIXED_MESSAGE else ""
        print(f"       {i + 1}: {decoded_text}{marker}")

    print("\n" + "=" * 60)

# problema 3
if __name__ == "__main__":
    # main()
    print(1528**2765%3589)