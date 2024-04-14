#ifndef BIGINT_H
#define BIGINT_H

#include <bits/stdc++.h>
using namespace std;

class BigInt {
private:
    vector<int> digits;
    bool is_negative;

    void normalize();

public:
    BigInt();
    BigInt(string str);
    BigInt(int num);

    operator int() const;
    operator string() const;

    bool operator<(const BigInt& other) const;
    bool operator>(const BigInt& other) const;
    bool operator<=(const BigInt& other) const;
    bool operator>=(const BigInt& other) const;
    bool operator==(const BigInt& other) const;
    bool operator!=(const BigInt& other) const;
    
    friend BigInt operator+(const BigInt& lhs, const BigInt& rhs);
    friend BigInt operator-(const BigInt& lhs, const BigInt& rhs);
    friend BigInt operator*(const BigInt& lhs, const BigInt& rhs);
    friend BigInt operator/(const BigInt& lhs, const BigInt& rhs);
    friend BigInt operator%(const BigInt& lhs, const BigInt& rhs);
    friend BigInt operator^(const BigInt& lhs, const BigInt& rhs);
    
    friend std::ostream& operator<<(std::ostream& os, const BigInt& num);
};

#endif
