#include "BigInt.h"

void BigInt::normalize() {
    while (digits.size() > 1 && digits.back() == 0) {
        digits.pop_back();
    }
    if (digits.empty()) {
        is_negative = false;
    }
}

BigInt::BigInt() : is_negative(false) {
    digits.push_back(0);
}

BigInt::BigInt(string str) {
    is_negative = (str[0] == '-');
    for (int i = (int)str.size() - 1; i >= is_negative; i--) {
        digits.push_back(str[i] - '0');
    }
    normalize();
}

BigInt::BigInt(int num) {
    is_negative = num < 0;
    num = abs(num);
    while (num) {
        digits.push_back(num % 10);
        num /= 10;
    }
    if (digits.empty()) {
        digits.push_back(0);
    }
}

BigInt::operator int() const {
    int num = 0;
    for (int i = digits.size() - 1; i >= 0; i--) {
        num = num * 10 + digits[i];
    }
    return is_negative ? -num : num;
}

BigInt::operator string() const {
    string str = is_negative ? "-" : "";
    for (int i = digits.size() - 1; i >= 0; i--) {
        str += to_string(digits[i]);
    }
    return str;
}

bool BigInt::operator<(const BigInt& other) const {
    if (is_negative != other.is_negative) {
        return is_negative;
    }
    if (digits.size() != other.digits.size()) {
        return is_negative ? digits.size() > other.digits.size() : digits.size() < other.digits.size();
    }
    for (int i = digits.size() - 1; i >= 0; i--) {
        if (digits[i] != other.digits[i]) {
            return is_negative ? digits[i] > other.digits[i] : digits[i] < other.digits[i];
        }
    }
    return false;
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

bool BigInt::operator==(const BigInt& other) const {
    return is_negative == other.is_negative && digits == other.digits;
}

bool BigInt::operator!=(const BigInt& other) const {
    return !(*this == other);
}

BigInt operator+(const BigInt& lhs, const BigInt& rhs) {
    BigInt result;
    if (lhs.is_negative == rhs.is_negative) {
        result.is_negative = lhs.is_negative;
    } else {
        if (lhs.is_negative) {
            return rhs - BigInt(-lhs);
        } else {
            return lhs - BigInt(-rhs);
        }
    }
    int carry = 0;
    int i = 0;
    while (i < (int)max(lhs.digits.size(), rhs.digits.size()) || carry) {
        int sum = carry;
        if (i < (int)lhs.digits.size()) {
            sum += lhs.digits[i];
        }
        if (i < (int)rhs.digits.size()) {
            sum += rhs.digits[i];
        }
        result.digits.push_back(sum % 10);
        carry = sum / 10;
        i++;
    }
    result.normalize();
    if (result.digits.size() > 1 && result.digits.front() == 0)
        result.digits.erase(result.digits.begin());
    return result;
}

BigInt operator-(const BigInt& lhs, const BigInt& rhs) {
    BigInt result;
    if (lhs.is_negative != rhs.is_negative) {
        result.is_negative = lhs.is_negative;
        return lhs + BigInt(-rhs);
    } else {
        if (lhs.is_negative) {
            if (rhs > lhs)
                return -(BigInt(-lhs) + rhs);
            else
                return (BigInt(-lhs) + rhs);
        } else {
            if (lhs < rhs) {
                result.is_negative = true;
                return -(rhs - lhs);
            } else {
                result.is_negative = false;
            }
        }
    }
    int borrow = 0;
    int i = 0;
    while (i < (int)lhs.digits.size() || borrow) {
        int diff = borrow;
        if (i < (int)lhs.digits.size()) {
            diff += lhs.digits[i];
        }
        if (i < (int)rhs.digits.size()) {
            diff -= rhs.digits[i];
        }
        result.digits.push_back((diff + 10) % 10);
        borrow = diff < 0 ? -1 : 0;
        i++;
    }
    result.normalize();
    if (result.digits.size() > 1 && result.digits.front() == 0)
        result.digits.erase(result.digits.begin());
    return result;
}

BigInt operator*(const BigInt& lhs, const BigInt& rhs) {
    BigInt result;
    result.digits.resize(lhs.digits.size() + rhs.digits.size(), 0);

    for (int i = 0; i < lhs.digits.size(); i++) {
        for (int j = 0; j < rhs.digits.size(); j++) {
            result.digits[i + j] += lhs.digits[i] * rhs.digits[j];
            result.digits[i + j + 1] += result.digits[i + j] / 10;
            result.digits[i + j] %= 10;
        }
    }

    result.is_negative = lhs.is_negative != rhs.is_negative;
    result.normalize();
    return result;
}

BigInt operator/(const BigInt& lhs, const BigInt& rhs) {
    if (rhs == BigInt(0)) {
        throw runtime_error("Division by zero");
    }
    
    BigInt abs_lhs;
    BigInt abs_rhs;
    if (lhs < BigInt(0)) {
        abs_lhs = -lhs;
    } else {
        abs_lhs = lhs;
    }
    
    if (rhs < BigInt(0)) {
        abs_rhs = -rhs;
    } else {
        abs_rhs = rhs;
    }
    
    if (abs_lhs < abs_rhs) {
        return BigInt(0);
    }
    
    BigInt result;
    result.is_negative = lhs.is_negative != rhs.is_negative;
    BigInt temp = 0;
    for (int i = (int)abs_lhs.digits.size() - 1; i >= 0; i--) {
        temp = temp * BigInt(10) + BigInt(abs_lhs.digits[i]);
        int div = 0;
        while (temp >= abs_rhs) {
            temp = temp - abs_rhs;
            div++;
        }
        result.digits.insert(result.digits.begin(), div);
    }
    result.normalize();
    return result;
}

BigInt operator%(const BigInt& lhs, const BigInt& rhs) {
    if (rhs == BigInt(0)) {
        throw runtime_error("Division by zero");
    }
    BigInt result = lhs - (lhs / rhs) * rhs;
    result.normalize();
    return result;
}

BigInt operator^(const BigInt& lhs, const BigInt& rhs) {
    if (rhs.is_negative) {
        throw runtime_error("Negative exponent");
    }
    BigInt result = 1;
    BigInt base = lhs;
    BigInt new_rhs = rhs;
    while (new_rhs > BigInt(0)) {
        if (new_rhs % BigInt(2) == BigInt(1)) {
            result = result * base;
        }
        base = base * base;
        new_rhs = new_rhs / BigInt(2);
    }
    return result;
}

std::ostream& operator<<(std::ostream& os, const BigInt& num) {
    if (num.is_negative) {
        os << "-";
    }
    for (int i = (int)num.digits.size() - 1; i >= 0; i--) {
        os << num.digits[i];
    }
    return os;
}
