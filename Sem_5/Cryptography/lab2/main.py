import string


def get_char_index(char):
    return ord(char) - ord('A')


def get_index_char(index):
    return chr(index + ord('A'))


def encrypt_belaso(plaintext, key):
    """
    Encrypts the plaintext using the Belaso (Vigenère) cipher and a keyword.

    Args:
        plaintext (str): The message to encrypt.
        key (str): The keyword to use for polyalphabetic substitution.

    Returns:
        str: The resulting ciphertext.
    """
    plaintext = plaintext.upper()
    key = key.upper().strip()

    key_alpha = "".join(c for c in key if c.isalpha())
    if not key_alpha:
        print("Error: The key must contain at least one letter.")
        return plaintext

    ciphertext = []
    key_length = len(key_alpha)
    key_index = 0

    for char in plaintext:
        if char.isalpha():
            P = get_char_index(char)
            K = get_char_index(key_alpha[key_index % key_length])

            C = (P + K) % 26

            ciphertext.append(get_index_char(C))

            key_index += 1
        else:
            ciphertext.append(char)

    return "".join(ciphertext)


def decrypt_belaso(ciphertext, key):
    """
    Decrypts the ciphertext using the Belaso (Vigenère) cipher and a keyword.

    Args:
        ciphertext (str): The message to decrypt.
        key (str): The keyword used during encryption.

    Returns:
        str: The resulting plaintext.
    """
    ciphertext = ciphertext.upper()
    key = key.upper().strip()

    key_alpha = "".join(c for c in key if c.isalpha())
    if not key_alpha:
        print("Error: The key must contain at least one letter.")
        return ciphertext

    plaintext = []
    key_length = len(key_alpha)
    key_index = 0

    for char in ciphertext:
        if char.isalpha():
            C = get_char_index(char)
            K = get_char_index(key_alpha[key_index % key_length])

            P = (C - K + 26) % 26

            plaintext.append(get_index_char(P))

            key_index += 1
        else:
            plaintext.append(char)

    return "".join(plaintext)


def main():
    original_text = "Everything is awesome"
    cipher_key = "Happy"

    print(f"\nOriginal Text: {original_text}")
    print(f"Key:           {cipher_key}")

    encrypted_message = encrypt_belaso(original_text, cipher_key)
    print(f"\nEncrypted Text: {encrypted_message}")

    decrypted_message = decrypt_belaso(encrypted_message, cipher_key)
    print(f"\nDecrypted Text: {decrypted_message}")

    if decrypted_message == original_text.upper():
        print("\nSuccess: Decrypted text matches the original (in uppercase).")
    else:
        print("\nFailure: Decrypted text does NOT match the original.")


if __name__ == "__main__":
    main()

# problema 4