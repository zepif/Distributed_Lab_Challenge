#include "BigInt.h"
using namespace std;

int main() {
    BigInt a("123456789012345678901234567890");
    BigInt b("987654321098765432109876543210");
    BigInt c = -45; // int -> BigInt
    string s = "50"; // string -> BigInt
    BigInt d(s);

    cout << "a = " << a << endl;
    cout << "b = " << b << endl;

    int x = a; // BigInt -> int
    string y = b; // BigInt -> string

    cout << "BigInt -> int: x = " << x << endl;
    cout << "BitInt -> string: y = " << y << endl << endl;

    cout << "c = " << c << endl;
    cout << "d = " << d << endl << endl;
    
    cout << "c + d = " << c + d << endl;
    cout << "c - d = " << c - d << endl;
    cout << "c * d = " << c * d << endl;
    cout << "c / d = " << c / d << endl;
    cout << "c % d = " << c % d << endl << endl;
    c = 45;
    d = 50;
    cout << "c = " << c << endl;
    cout << "d = " << d << endl << endl;
    
    cout << "c + d = " << c + d << endl;
    cout << "c - d = " << c - d << endl;
    cout << "c * d = " << c * d << endl;
    cout << "c / d = " << c / d << endl;
    cout << "c % d = " << c % d << endl << endl;

    BigInt power = c ^ d;
    cout << "c^d = " << power << endl << endl;

    cout << "c > d = " << (c > d) << endl;
    cout << "c < d = " << (c < d) << endl;
    cout << "c >= d = " << (c >= d) << endl;
    cout << "c <= d = " << (c <= d) << endl << endl;
    
    c = 45;
    d = -30;
    cout << "c = " << c << endl;
    cout << "d = " << d << endl << endl;
    cout << "c + d = " << c + d << endl;
    cout << "c / d = " << c / d << endl;
    cout << "c % d = " << c % d << endl << endl;


    return 0;
}
