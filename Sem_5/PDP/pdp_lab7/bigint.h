#ifndef BIGINT_H
#define BIGINT_H

#include <vector>
#include <string>
#include <iostream>

class BigInt {
public:
    // Constructors
    BigInt();
    BigInt(int value);
    BigInt(const std::string& value);
    BigInt(const std::vector<int>& digits, bool negative = false);
    
    // Copy constructor and assignment
    BigInt(const BigInt& other) = default;
    BigInt& operator=(const BigInt& other) = default;
    
    // Arithmetic operations
    BigInt operator+(const BigInt& other) const;
    BigInt operator-(const BigInt& other) const;
    BigInt operator*(const BigInt& other) const;
    BigInt operator-() const; // Unary minus
    
    // Comparison operators
    bool operator==(const BigInt& other) const;
    bool operator<(const BigInt& other) const;
    bool operator>(const BigInt& other) const;
    bool operator<=(const BigInt& other) const;
    bool operator>=(const BigInt& other) const;
    
    // Utility
    std::string toString() const;
    bool isNegative() const;
    int getDigit(int index) const;
    size_t getSize() const;
    void trim(); // Remove leading zeros
    
    // For polynomial multiplication
    static BigInt fromInt(int value);
    
private:
    std::vector<int> digits_; // Stored in little-endian format (least significant first)
    bool negative_;
    
    void normalize();
    int compareMagnitude(const BigInt& other) const; // Compare absolute values
    BigInt addMagnitude(const BigInt& other) const; // Add absolute values
    BigInt subtractMagnitude(const BigInt& other) const; // Subtract absolute values (assumes this >= other)
};

#endif // BIGINT_H

