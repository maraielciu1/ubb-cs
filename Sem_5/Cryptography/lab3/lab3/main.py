import math
import random


def sieve_of_eratosthenes(limit):
    """
    Generates all prime numbers up to the smoothness bound 'limit' (B).
    """
    if limit < 2:
        return []
    is_prime = [True] * (limit + 1)
    is_prime[0] = is_prime[1] = False

    for p in range(2, int(math.sqrt(limit)) + 1):
        if is_prime[p]:
            for multiple in range(p * p, limit + 1, p):
                is_prime[multiple] = False

    primes = [p for p, is_p in enumerate(is_prime) if is_p]
    return primes


def pollard_p_minus_1(n, B=1000, max_retries=5):
    """
    Pollard's p-1 Factorization Algorithm.

    Tries to find a non-trivial factor of n, using a prime smoothness bound B.

    Args:
        n (int): The composite number to be factored.
        B (int): The prime smoothness bound (user-defined or default 1000).
        max_retries (int): Max attempts with different starting bases 'a'.

    Returns:
        int: A non-trivial factor d of n (1 < d < n), or -1 if no factor is found.
    """

    if n <= 3: return -1
    if n % 2 == 0: return 2

    print(f"Starting Pollard's p-1 for N={n} with Bound B={B}")

    # Generate primes up to the bound B
    primes = sieve_of_eratosthenes(B)

    for attempt in range(max_retries):
        # Step 1: Initialization
        # Use a random base 'a' for subsequent retries, starting with 2 for the first attempt.
        a = random.randint(2, n - 2) if attempt > 0 else 2

        # Check trivial GCD
        d = math.gcd(a, n)
        if d > 1:
            print(f"Attempt {attempt + 1}: Found factor {d} in initial GCD check.")
            return d

        x = a  # x holds the accumulating modular exponentiation result (a^K mod n)

        print(f"Attempt {attempt + 1}: Base a={a}. Calculating x = a^K mod n...")

        # Step 2 & 3: Iterative Exponentiation and GCD Check
        for q in primes:
            # Determine the highest power of q, q^e, such that q^e <= B
            e = math.floor(math.log(B) / math.log(q))
            q_power = q ** e

            # Compute x = x^(q^e) mod n (which is equivalent to a^K mod n)
            # Python's pow(base, exp, mod) handles large integers efficiently.
            x = pow(x, q_power, n)

            # Check for factor after incorporating this prime power
            d = math.gcd(x - 1, n)

            if 1 < d < n:
                print(f"Attempt {attempt + 1}: Factor found: {d}.")
                return d

            if d == n:
                # Failure: a^K == 1 mod n for *all* factors. Need a new base 'a'.
                print(f"Attempt {attempt + 1}: Trivial factor N found. Retrying with a new base.")
                break  # Exit prime loop and try a new base 'a'

        # If the inner loop completes without a factor, the bound B was likely too small.
        if d == 1:
            print(
                f"Attempt {attempt + 1}: No factor found. B={B} may be too small or composite is not p-1 smooth. Retrying with a new base.")
        # If the inner loop broke with d=n, the outer loop continues for a new attempt.

    print(f"\nFailed to find a factor after {max_retries} attempts with B={B}. Consider increasing B.")
    return -1


def main():
    """
    Console application driver for Pollard's p-1 algorithm.
    """
    print("    Pollard's p-1 Factorization Algorithm     ")

    while True:
        try:
            n_str = input("\nEnter the composite number N to factor (or 'q' to quit): ")
            if n_str.lower() == 'q':
                break

            n = int(n_str)
            if n < 4:
                print("N must be a composite number greater than 3.")
                continue

            # Determine the smoothness bound B
            default_b = 1000
            use_default = input(f"Use default bound B={default_b}? (Y/n, default Y): ").strip().lower()

            B = default_b
            if use_default == 'n':
                b_str = input("Enter custom bound B (integer): ").strip()
                try:
                    B = int(b_str)
                    if B < 2:
                        print("Bound B must be at least 2.")
                        continue
                except ValueError:
                    print("Invalid input for B. Using default B=1000.")
                    B = default_b

            # Run the factorization
            factor = pollard_p_minus_1(n, B=B)

            # Display final result
            if factor != -1:
                print(f"SUCCESS: A non-trivial factor of {n} is {factor}.")
                other_factor = n // factor
                print(f"The factorization is: {factor} * {other_factor}")
            else:
                print(f"FAILURE: Could not find a factor for N={n} with B={B} in 5 attempts.")
                print("Try increasing the bound B.")
            print("----------------------------------------------")


        except ValueError:
            print("Invalid input. Please enter a valid integer for N.")
        except Exception as e:
            print(f"An unexpected error occurred: {e}")

    print("\nThank you for using the Pollard's p-1 Factorization tool. Goodbye!")


if __name__ == "__main__":
    main()

# n=1241143 B=13 a=2
