#include "bigint.h"
#include <algorithm>
#include <stdexcept>
#include <cctype>

const int BASE = 10; // Base 10: each element stores a single digit (0-9)

BigInt::BigInt() : negative_(false) {
    digits_.push_back(0);
}

BigInt::BigInt(int value) : negative_(value < 0) {
    if (value == 0) {
        digits_.push_back(0);
    } else {
        int absValue = value < 0 ? -value : value;
        while (absValue > 0) {
            digits_.push_back(absValue % 10);  // Single digit
            absValue /= 10;
        }
    }
}

BigInt::BigInt(const std::string& value) : negative_(false) {
    if (value.empty()) {
        digits_.push_back(0);
        return;
    }
    
    size_t start = 0;
    if (value[0] == '-') {
        negative_ = true;
        start = 1;
    } else if (value[0] == '+') {
        start = 1;
    }
    
    if (start >= value.length() || value.find_first_not_of("0123456789", start) != std::string::npos) {
        throw std::invalid_argument("Invalid BigInt string: " + value);
    }
    
    // Parse from right to left, one digit at a time
    for (int i = static_cast<int>(value.length()) - 1; i >= static_cast<int>(start); --i) {
        if (!std::isdigit(value[i])) {
            throw std::invalid_argument("Invalid character in BigInt: " + value);
        }
        digits_.push_back(value[i] - '0');  // Store single digit (0-9)
    }
    
    trim();
    if (digits_.size() == 1 && digits_[0] == 0) {
        negative_ = false;
    }
}

BigInt::BigInt(const std::vector<int>& digits, bool negative) 
    : digits_(digits), negative_(negative) {
    trim();
    if (digits_.size() == 1 && digits_[0] == 0) {
        negative_ = false;
    }
}

void BigInt::trim() {
    while (digits_.size() > 1 && digits_.back() == 0) {
        digits_.pop_back();
    }
    if (digits_.empty()) {
        digits_.push_back(0);
        negative_ = false;
    }
}

void BigInt::normalize() {
    trim();
    if (digits_.size() == 1 && digits_[0] == 0) {
        negative_ = false;
    }
}

std::string BigInt::toString() const {
    if (digits_.size() == 1 && digits_[0] == 0) {
        return "0";
    }
    
    std::string result;
    if (negative_) {
        result += "-";
    }
    
    // Convert digits from most significant to least significant
    // (digits are stored least significant first, so iterate backwards)
    for (int i = static_cast<int>(digits_.size()) - 1; i >= 0; --i) {
        result += std::to_string(digits_[i]);  // Each digit is 0-9, no padding needed
    }
    
    return result;
}

bool BigInt::isNegative() const {
    return negative_;
}

int BigInt::getDigit(int index) const {
    if (index < 0 || index >= static_cast<int>(digits_.size())) {
        return 0;
    }
    return digits_[index];
}

size_t BigInt::getSize() const {
    return digits_.size();
}

BigInt BigInt::fromInt(int value) {
    return BigInt(value);
}

int BigInt::compareMagnitude(const BigInt& other) const {
    if (digits_.size() < other.digits_.size()) return -1;
    if (digits_.size() > other.digits_.size()) return 1;
    
    for (int i = static_cast<int>(digits_.size()) - 1; i >= 0; --i) {
        if (digits_[i] < other.digits_[i]) return -1;
        if (digits_[i] > other.digits_[i]) return 1;
    }
    
    return 0;
}

BigInt BigInt::addMagnitude(const BigInt& other) const {
    BigInt result;
    result.digits_.clear();
    result.negative_ = false;
    
    int carry = 0;
    size_t maxSize = std::max(digits_.size(), other.digits_.size());
    
    for (size_t i = 0; i < maxSize || carry > 0; ++i) {
        int sum = carry;
        if (i < digits_.size()) sum += digits_[i];
        if (i < other.digits_.size()) sum += other.digits_[i];
        
        result.digits_.push_back(sum % BASE);
        carry = sum / BASE;
    }
    
    result.normalize();
    return result;
}

BigInt BigInt::subtractMagnitude(const BigInt& other) const {
    // Assumes this >= other in magnitude
    BigInt result;
    result.digits_.clear();
    result.negative_ = false;
    
    int borrow = 0;
    for (size_t i = 0; i < digits_.size(); ++i) {
        int diff = digits_[i] - borrow;
        if (i < other.digits_.size()) {
            diff -= other.digits_[i];
        }
        
        if (diff < 0) {
            diff += BASE;
            borrow = 1;
        } else {
            borrow = 0;
        }
        
        result.digits_.push_back(diff);
    }
    
    result.normalize();
    return result;
}

BigInt BigInt::operator-() const {
    BigInt result = *this;
    if (result.digits_.size() > 1 || result.digits_[0] != 0) {
        result.negative_ = !result.negative_;
    }
    return result;
}

bool BigInt::operator==(const BigInt& other) const {
    return negative_ == other.negative_ && digits_ == other.digits_;
}

bool BigInt::operator<(const BigInt& other) const {
    if (negative_ != other.negative_) {
        return negative_;
    }
    
    int cmp = compareMagnitude(other);
    if (cmp == 0) return false;
    
    return negative_ ? (cmp > 0) : (cmp < 0);
}

bool BigInt::operator>(const BigInt& other) const {
    return other < *this;
}

bool BigInt::operator<=(const BigInt& other) const {
    return !(*this > other);
}

bool BigInt::operator>=(const BigInt& other) const {
    return !(*this < other);
}

BigInt BigInt::operator+(const BigInt& other) const {
    if (negative_ == other.negative_) {
        // Same sign: add magnitudes
        BigInt result = addMagnitude(other);
        result.negative_ = negative_;
        return result;
    } else {
        // Different signs: subtract magnitudes
        int cmp = compareMagnitude(other);
        if (cmp == 0) {
            return BigInt(0);
        } else if (cmp > 0) {
            BigInt result = subtractMagnitude(other);
            result.negative_ = negative_;
            return result;
        } else {
            BigInt result = other.subtractMagnitude(*this);
            result.negative_ = other.negative_;
            return result;
        }
    }
}

BigInt BigInt::operator-(const BigInt& other) const {
    return *this + (-other);
}

BigInt BigInt::operator*(const BigInt& other) const {
    if (digits_.size() == 1 && digits_[0] == 0) {
        return BigInt(0);
    }
    if (other.digits_.size() == 1 && other.digits_[0] == 0) {
        return BigInt(0);
    }
    
    BigInt result;
    result.digits_.resize(digits_.size() + other.digits_.size(), 0);
    result.negative_ = (negative_ != other.negative_);
    
    for (size_t i = 0; i < digits_.size(); ++i) {
        long long carry = 0;
        for (size_t j = 0; j < other.digits_.size() || carry > 0; ++j) {
            long long product = static_cast<long long>(result.digits_[i + j]) + 
                               static_cast<long long>(digits_[i]) * 
                               (j < other.digits_.size() ? other.digits_[j] : 0) + 
                               carry;
            result.digits_[i + j] = static_cast<int>(product % BASE);
            carry = product / BASE;
        }
    }
    
    result.normalize();
    return result;
}

