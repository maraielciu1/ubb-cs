#ifndef POLYNOMIAL_H
#define POLYNOMIAL_H

#include <vector>
#include <iostream>
#include <string>

class Polynomial {
public:
    // Constructors
    Polynomial() = default;
    explicit Polynomial(const std::vector<int>& coefficients);
    explicit Polynomial(int degree); // Creates polynomial with given degree (all zeros)
    
    // Accessors
    int getDegree() const;
    int getCoefficient(int index) const;
    void setCoefficient(int index, int value);
    const std::vector<int>& getCoefficients() const;
    
    // Operations
    Polynomial operator+(const Polynomial& other) const;
    Polynomial operator-(const Polynomial& other) const;
    Polynomial shiftLeft(int positions) const; // Multiply by x^positions
    
    // Utility
    void resize(int newSize);
    void trim(); // Remove leading zeros
    std::string toString() const;
    
private:
    std::vector<int> coefficients_;
};

// Regular O(n²) sequential multiplication
Polynomial multiplyRegularSequential(const Polynomial& a, const Polynomial& b);

// Regular O(n²) parallel multiplication (CPU threads)
Polynomial multiplyRegularParallel(const Polynomial& a, const Polynomial& b);

// Karatsuba sequential multiplication
Polynomial multiplyKaratsubaSequential(const Polynomial& a, const Polynomial& b);

// Karatsuba parallel multiplication (CPU threads)
Polynomial multiplyKaratsubaParallel(const Polynomial& a, const Polynomial& b);

#endif // POLYNOMIAL_H

