"""
Cramer-Shoup Cryptosystem Implementation

A public-key encryption scheme with IND-CCA2 security based on the DDH assumption.
Uses a 27-character alphabet: space (0) and A-Z (1-26).
"""

import random
import hashlib
from typing import Tuple


# ALPHABET CONFIGURATION (27 characters: space + A-Z)

ALPHABET_SIZE = 27

def char_to_num(character: str) -> int:
    """
    Convert a character to its numeric value.
    
    Steps:
    1. Convert character to uppercase for case-insensitive handling
    2. Map space to 0, A-Z to 1-26
    3. Raise error for invalid characters
    """
    character_upper = character.upper()
    if character_upper == ' ': 
        return 0
    if 'A' <= character_upper <= 'Z': 
        return ord(character_upper) - ord('A') + 1
    raise ValueError(f"Invalid character: '{character}'. Only space and A-Z allowed.")

def num_to_char(numeric_value: int) -> str:
    """
    Convert a numeric value to its corresponding character.
    
    Steps:
    1. Map 0 to space
    2. Map 1-26 to A-Z
    3. Raise error for invalid numeric values
    """
    if numeric_value == 0: 
        return ' '
    if 1 <= numeric_value <= 26: 
        return chr(ord('A') + numeric_value - 1)
    raise ValueError(f"Invalid number: {numeric_value}. Must be 0-26.")

def text_to_number(plaintext: str) -> int:
    """
    Convert plaintext string to integer using base-27 encoding.
    
    Steps:
    1. For each character in the plaintext (left to right):
       - Convert character to its numeric value
       - Multiply current result by 27 (base shift)
       - Add the character's numeric value
    2. Return the final encoded integer
    """
    encoded_message = 0
    for character in plaintext:
        encoded_message = encoded_message * ALPHABET_SIZE + char_to_num(character)
    return encoded_message

def number_to_text(encoded_message: int, message_length: int) -> str:
    """
    Convert integer back to plaintext string using base-27 decoding.
    
    Steps:
    1. Extract each digit (character value) by taking modulo 27
    2. Convert each digit to its character representation
    3. Reverse the list since we extract from least significant to most significant
    4. Join characters to form the original plaintext
    """
    decoded_characters = []
    remaining_value = encoded_message
    for _ in range(message_length):
        character_value = remaining_value % ALPHABET_SIZE
        decoded_characters.append(num_to_char(character_value))
        remaining_value //= ALPHABET_SIZE
    return ''.join(reversed(decoded_characters))


def is_prime_miller_rabin(candidate: int, rounds: int = 20) -> bool:
    """
    Miller-Rabin primality test to check if a number is probably prime.
    
    Steps:
    1. Handle edge cases (n < 2, even numbers, n == 3)
    2. Write n-1 as 2^r * d where d is odd
    3. For each round:
       a. Pick a random witness a in [2, n-2]
       b. Compute x = a^d mod n
       c. If x == 1 or x == n-1, candidate passes this round
       d. Otherwise, square x repeatedly (r-1 times) to check for n-1
       e. If n-1 never appears, candidate is composite
    4. If all rounds pass, candidate is probably prime
    """
    if candidate < 2 or candidate % 2 == 0: 
        return candidate == 2
    if candidate == 3: 
        return True
    
    # Write candidate-1 as 2^r * d where d is odd
    exponent_power = 0
    odd_component = candidate - 1
    while odd_component % 2 == 0:
        exponent_power += 1
        odd_component //= 2
    
    # Perform rounds of testing
    for _ in range(rounds):
        random_witness = random.randrange(2, candidate - 1)
        witness_power = pow(random_witness, odd_component, candidate)
        
        if witness_power == 1 or witness_power == candidate - 1:
            continue
        
        # Square repeatedly to check for candidate-1
        found_negative_one = False
        for _ in range(exponent_power - 1):
            witness_power = pow(witness_power, 2, candidate)
            if witness_power == candidate - 1:
                found_negative_one = True
                break
        
        if not found_negative_one:
            return False
    
    return True

def generate_safe_prime(bit_length: int) -> Tuple[int, int]:
    """
    Generate a safe prime p = 2q + 1 where q is also prime.
    
    Steps:
    1. Generate random candidate q with specified bit length
    2. Ensure q is odd and large enough (set high bit)
    3. Test if q is prime using Miller-Rabin
    4. If q is prime, compute p = 2q + 1
    5. Test if p is also prime
    6. Return (p, q) when both are prime
    """
    print(f"Generating {bit_length}-bit safe prime...")
    while True:
        # Generate candidate for q (Sophie Germain prime)
        candidate_q = random.getrandbits(bit_length - 1)
        # Ensure it's large enough and odd
        candidate_q |= (1 << (bit_length - 2)) | 1
        
        if is_prime_miller_rabin(candidate_q):
            # Check if p = 2q + 1 is also prime
            candidate_p = 2 * candidate_q + 1
            if is_prime_miller_rabin(candidate_p):
                print(f"Found safe prime with {candidate_p.bit_length()} bits")
                return candidate_p, candidate_q

def find_generator(prime_modulus: int, subgroup_order: int) -> int:
    """
    Find a generator of the subgroup of order q in Z*_p.
    
    Steps:
    1. Pick a random element h in Z*_p (range [2, p-1])
    2. Compute g = h^2 mod p (this ensures g is in the subgroup of order q)
    3. Verify g != 1 (to ensure it's a valid generator)
    4. Return g
    """
    while True:
        random_element = random.randrange(2, prime_modulus - 1)
        generator_candidate = pow(random_element, 2, prime_modulus)
        if generator_candidate != 1:
            return generator_candidate


# one way hash function

def hash_function(commitment_g1: int, commitment_g2: int, encrypted_message: int, subgroup_order: int) -> int:
    """
    Collision-resistant hash function for Cramer-Shoup verification.
    Maps (u1, u2, e) to an element in Z_q.
    
    Steps:
    1. Concatenate u1, u2, and e with separators
    2. Encode the string to bytes
    3. Compute SHA-256 hash
    4. Convert hash bytes to integer
    5. Reduce modulo q to get value in Z_q
    """
    concatenated_data = f"{commitment_g1}|{commitment_g2}|{encrypted_message}".encode('utf-8')
    hash_bytes = hashlib.sha256(concatenated_data).digest()
    hash_integer = int.from_bytes(hash_bytes, 'big')
    return hash_integer % subgroup_order


# CRAMER-SHOUP CRYPTOSYSTEM

class CramerShoupPublicKey:
    """
    Public key for Cramer-Shoup cryptosystem.
    Contains: (p, q, g1, g2, c, d, h)
    """
    def __init__(self, prime_modulus: int, subgroup_order: int, generator_1: int, 
                 generator_2: int, public_value_c: int, public_value_d: int, public_value_h: int):
        self.p = prime_modulus          # Safe prime modulus
        self.q = subgroup_order         # Order of the subgroup
        self.g1 = generator_1          # First generator
        self.g2 = generator_2          # Second generator
        self.c = public_value_c         # c = g1^x1 * g2^x2
        self.d = public_value_d         # d = g1^y1 * g2^y2
        self.h = public_value_h         # h = g1^z
    
    def __str__(self):
        return f"Public Key: p={self.p}, q={self.q}, g1={self.g1}, g2={self.g2}, c={self.c}, d={self.d}, h={self.h}"

class CramerShoupPrivateKey:
    """
    Private key for Cramer-Shoup cryptosystem.
    Contains: (x1, x2, y1, y2, z)
    """
    def __init__(self, secret_exponent_x1: int, secret_exponent_x2: int, 
                 secret_exponent_y1: int, secret_exponent_y2: int, 
                 secret_exponent_z: int, public_key: CramerShoupPublicKey):
        self.x1 = secret_exponent_x1    # Secret exponent for c (with g1)
        self.x2 = secret_exponent_x2    # Secret exponent for c (with g2)
        self.y1 = secret_exponent_y1    # Secret exponent for d (with g1)
        self.y2 = secret_exponent_y2    # Secret exponent for d (with g2)
        self.z = secret_exponent_z      # Secret exponent for h
        self.public_key = public_key     # Associated public key
    
    def __str__(self):
        return f"Private Key: x1={self.x1}, x2={self.x2}, y1={self.y1}, y2={self.y2}, z={self.z}"

class CramerShoupCiphertext:
    """
    Ciphertext for Cramer-Shoup cryptosystem.
    Contains: (u1, u2, e, v, length)
    """
    def __init__(self, commitment_g1: int, commitment_g2: int, encrypted_message: int, 
                 verification_tag: int, plaintext_length: int):
        self.u1 = commitment_g1         # g1^k (commitment to random exponent)
        self.u2 = commitment_g2         # g2^k (commitment to random exponent)
        self.e = encrypted_message      # m * h^k (message masked with shared secret)
        self.v = verification_tag       # Verification tag for integrity
        self.length = plaintext_length  # Original plaintext length
    
    def __str__(self):
        return f"Ciphertext: u1={self.u1}, u2={self.u2}, e={self.e}, v={self.v}, length={self.length}"


def generate_keys(bits: int = 256) -> Tuple[CramerShoupPublicKey, CramerShoupPrivateKey]:
    """
    Generate public and private key pair (randomly generated in required interval).
    
    Steps:
    1. Generate safe prime p = 2q + 1
    2. Find two independent generators g1 and g2 of the subgroup of order q
    3. Randomly select private exponents x1, x2, y1, y2, z from [1, q-1]
    4. Compute public values:
       - c = g1^x1 * g2^x2 (mod p)
       - d = g1^y1 * g2^y2 (mod p)
       - h = g1^z (mod p)
    5. Create and return public and private key objects
    """
    print("\n")
    print("Key Generation")
    print("." * 60)
    
    # Step 1: Generate safe prime
    prime_modulus, subgroup_order = generate_safe_prime(bits)
    
    # Step 2: Find two independent generators
    generator_1 = find_generator(prime_modulus, subgroup_order)
    generator_2 = find_generator(prime_modulus, subgroup_order)
    while generator_2 == generator_1:
        generator_2 = find_generator(prime_modulus, subgroup_order)
    
    # Step 3: Generate random private exponents in Z_q
    secret_exponent_x1 = random.randrange(1, subgroup_order)
    secret_exponent_x2 = random.randrange(1, subgroup_order)
    secret_exponent_y1 = random.randrange(1, subgroup_order)
    secret_exponent_y2 = random.randrange(1, subgroup_order)
    secret_exponent_z = random.randrange(1, subgroup_order)
    
    # Step 4: Compute public values
    public_value_c = (pow(generator_1, secret_exponent_x1, prime_modulus) * 
                     pow(generator_2, secret_exponent_x2, prime_modulus)) % prime_modulus
    public_value_d = (pow(generator_1, secret_exponent_y1, prime_modulus) * 
                     pow(generator_2, secret_exponent_y2, prime_modulus)) % prime_modulus
    public_value_h = pow(generator_1, secret_exponent_z, prime_modulus)
    
    # Step 5: Create key objects
    public_key = CramerShoupPublicKey(prime_modulus, subgroup_order, generator_1, generator_2,
                                      public_value_c, public_value_d, public_value_h)
    private_key = CramerShoupPrivateKey(secret_exponent_x1, secret_exponent_x2,
                                        secret_exponent_y1, secret_exponent_y2,
                                        secret_exponent_z, public_key)
    
    print("Key generation complete!")
    print("." * 60 + "\n")
    return public_key, private_key


def validate_plaintext(plaintext: str) -> Tuple[bool, str]:
    """
    Validate that plaintext contains only valid characters.
    
    Steps:
    1. Check if plaintext is empty
    2. Convert to uppercase for case-insensitive validation
    3. Check each character:
       - Must be either space or letter A-Z
       - Report position of first invalid character if found
    4. Return validation result and message
    """
    if not plaintext:
        return False, "Plaintext cannot be empty."
    
    plaintext_upper = plaintext.upper()
    for position, character in enumerate(plaintext_upper):
        if character != ' ' and (character < 'A' or character > 'Z'):
            return False, f"Invalid character '{character}' at position {position}. Only space and A-Z allowed."
    
    return True, "Plaintext is valid."


def encrypt(plaintext: str, public_key: CramerShoupPublicKey) -> CramerShoupCiphertext:
    """
    Encrypt plaintext using public key (with plaintext validation).
    
    Steps:
    1. Validate plaintext (only space and A-Z allowed)
    2. Convert plaintext to uppercase
    3. Encode plaintext as integer using base-27 encoding
    4. Check that encoded message fits in the group (m < p)
    5. Select random ephemeral key k in [1, q-1]
    6. Compute ciphertext components:
       - u1 = g1^k (mod p) - commitment to random exponent
       - u2 = g2^k (mod p) - commitment to random exponent
       - e = m * h^k (mod p) - message masked with shared secret
       - α = H(u1, u2, e) - hash for integrity verification
       - v = c^k * d^(k*α) (mod p) - verification tag
    7. Return ciphertext with original plaintext length
    """
    print("\n")
    print("Encryption")
    print("." * 60)
    
    # Step 1: Plaintext validation
    is_valid, validation_message = validate_plaintext(plaintext)
    if not is_valid:
        raise ValueError(f"Plaintext validation failed: {validation_message}")
    
    print(f"Plaintext: '{plaintext}'")
    print(f"Validation: {validation_message}")
    
    # Step 2: Convert to uppercase
    plaintext_upper = plaintext.upper()
    
    # Step 3: Encode plaintext as integer
    encoded_message = text_to_number(plaintext_upper)
    
    # Step 4: Check message fits in group
    if encoded_message >= public_key.p:
        max_chars = (public_key.p.bit_length() - 1) // 5
        raise ValueError(f"Message too long! Maximum {max_chars} characters.")
    
    # Step 5: Select random ephemeral key
    ephemeral_key = random.randrange(1, public_key.q)
    
    # Step 6: Compute ciphertext components
    commitment_g1 = pow(public_key.g1, ephemeral_key, public_key.p)
    commitment_g2 = pow(public_key.g2, ephemeral_key, public_key.p)
    shared_secret = pow(public_key.h, ephemeral_key, public_key.p)
    encrypted_message = (encoded_message * shared_secret) % public_key.p
    
    integrity_hash = hash_function(commitment_g1, commitment_g2, encrypted_message, public_key.q)
    
    verification_tag_component_1 = pow(public_key.c, ephemeral_key, public_key.p)
    verification_tag_component_2 = pow(public_key.d, (ephemeral_key * integrity_hash) % public_key.q, public_key.p)
    verification_tag = (verification_tag_component_1 * verification_tag_component_2) % public_key.p
    
    print("Encryption complete!")
    print("." * 60 + "\n")
    
    return CramerShoupCiphertext(commitment_g1, commitment_g2, encrypted_message, 
                                 verification_tag, len(plaintext_upper))


def validate_ciphertext(ciphertext: CramerShoupCiphertext, public_key: CramerShoupPublicKey) -> Tuple[bool, str]:
    """
    Validate ciphertext structure and bounds.
    
    Steps:
    1. Check that all ciphertext components (u1, u2, e, v) are in valid range (0, p)
    2. Check that length field is positive
    3. Return validation result and message
    """
    if not (0 < ciphertext.u1 < public_key.p and 
            0 < ciphertext.u2 < public_key.p and 
            0 < ciphertext.e < public_key.p and 
            0 < ciphertext.v < public_key.p):
        return False, "Invalid ciphertext: values must be in range (0, p)"
    if ciphertext.length <= 0:
        return False, "Invalid ciphertext: length must be positive"
    return True, "Ciphertext structure is valid."


def decrypt(ciphertext: CramerShoupCiphertext, private_key: CramerShoupPrivateKey) -> str:
    """
    Decrypt ciphertext using private key (with ciphertext validation).
    
    Steps:
    1. Validate ciphertext structure (bounds and length)
    2. Compute integrity hash α = H(u1, u2, e)
    3. Verify ciphertext integrity:
       - Compute v' = u1^(x1 + y1*α) * u2^(x2 + y2*α) (mod p)
       - Check if v' == v
       - If not equal, reject (ciphertext was tampered)
    4. Recover message:
       - Compute shared secret component = u1^z (mod p)
       - Compute its modular inverse
       - Recover m = e * (u1^z)^(-1) (mod p)
    5. Decode integer back to plaintext using base-27 decoding
    6. Return decrypted plaintext
    """
    print("Decryption")
    print("." * 60)
    
    public_key = private_key.public_key
    
    # Step 1: Ciphertext validation
    is_valid, validation_message = validate_ciphertext(ciphertext, public_key)
    if not is_valid:
        raise ValueError(f"Ciphertext validation failed: {validation_message}")
    
    print(f"Ciphertext validation: {validation_message}")
    
    # Step 2: Compute integrity hash
    integrity_hash = hash_function(ciphertext.u1, ciphertext.u2, ciphertext.e, public_key.q)
    
    # Step 3: Verify ciphertext integrity
    verification_exponent_1 = (private_key.x1 + private_key.y1 * integrity_hash) % public_key.q
    verification_exponent_2 = (private_key.x2 + private_key.y2 * integrity_hash) % public_key.q
    computed_verification_tag = (pow(ciphertext.u1, verification_exponent_1, public_key.p) * 
                                 pow(ciphertext.u2, verification_exponent_2, public_key.p)) % public_key.p
    
    if computed_verification_tag != ciphertext.v:
        raise ValueError("DECRYPTION FAILED: Ciphertext verification failed! The ciphertext may have been tampered with.")
    
    print("Verification PASSED - ciphertext is authentic")
    
    # Step 4: Recover message
    shared_secret_component = pow(ciphertext.u1, private_key.z, public_key.p)
    shared_secret_inverse = pow(shared_secret_component, public_key.p - 2, public_key.p)
    encoded_message = (ciphertext.e * shared_secret_inverse) % public_key.p
    
    # Step 5: Decode integer back to plaintext
    decrypted_plaintext = number_to_text(encoded_message, ciphertext.length)
    
    print(f"Decrypted plaintext: '{decrypted_plaintext}'")
    print("Decryption complete!")
    print("." * 60 + "\n")
    
    return decrypted_plaintext


def demonstrate():
    """
    Demonstrate the cryptosystem with one success and one failure case.
    
    Steps:
    1. Display alphabet configuration
    2. Generate key pair
    3. Case 1: Successful encryption and decryption
    4. Case 2: Tampered ciphertext detection (failure case)
    """
    print("\n")
    print("CRAMER-SHOUP CRYPTOSYSTEM ")
    print("." * 70 + "\n")
    
    print("Alphabet: 27 characters (space + A-Z)")
    print("Encoding: Space=0, A=1, B=2, ..., Z=26\n")
    
    # Generate keys
    print("Step 1: Generating key pair...")
    public_key, private_key = generate_keys(bits=256)
    print(f"Public Key: p={public_key.p}, q={public_key.q}")
    print(f"Private Key: (x1, x2, y1, y2, z)\n")
    
    # Case 1: SUCCESS - Valid encryption and decryption
    print("Successful case - Valid Encryption and Decryption")
    print("." * 70)
    try:
        test_plaintext = "HELLO WORLD"
        print(f"\nPlaintext: '{test_plaintext}'")
        
        test_ciphertext = encrypt(test_plaintext, public_key)
        print(f"Ciphertext: (u1, u2, e, v, length)")
        
        decrypted_text = decrypt(test_ciphertext, private_key)
        
        if decrypted_text == test_plaintext.upper():
            print(f"Success: Decryption matches original!")
        else:
            print(f"Error: Mismatch! Original: '{test_plaintext.upper()}', Decrypted: '{decrypted_text}'")
    except ValueError as error:
        print(f"Error: {error}")
    
    # Case 2: FAILURE - Tampered ciphertext detection
    print("Fail Case - Tampered Ciphertext Detection")
    print("." * 70)
    try:
        test_plaintext = "SECRET MESSAGE"
        print(f"\nPlaintext: '{test_plaintext}'")
        
        original_ciphertext = encrypt(test_plaintext, public_key)
        
        # Tamper with ciphertext
        print("\nTampering with ciphertext (modifying 'e' value)...")
        tampered_ciphertext = CramerShoupCiphertext(
            original_ciphertext.u1,
            original_ciphertext.u2,
            (original_ciphertext.e + 1) % public_key.p,  # Tampered!
            original_ciphertext.v,
            original_ciphertext.length
        )
        
        decrypt(tampered_ciphertext, private_key)
        print(" Error: Tampered ciphertext was not detected!")
    except ValueError as error:
        print(f" Success: {error}")
    
    print("\n")
    print("Done")


if __name__ == "__main__":
    demonstrate()
