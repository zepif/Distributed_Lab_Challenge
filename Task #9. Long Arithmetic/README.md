# Long Arithmetic

This is a C++ implementation of a BigInt class that provides support for integer arithmetic.

### Features

- Supports addition, subtraction, multiplication, division, modulo, and exponentiation operations.

- Handles positive and negative integers.

- Provides conversion between `BigInt` and `int`, `string`.

- Follows standard operator overloading conventions.

### Usage

1. Include the `BigInt.h` header file in your C++ source code.

2. Create `BigInt` objects using the provided constructors:

  - Default constructor: `BigInt()` creates a `BigInt` with value 0.

  - String constructor: `BigInt(string str)` creates a `BigInt` from a string representation.

  - Integer constructor: `BigInt(int num)` creates a `BigInt` from an integer value.

3. Perform arithmetic operations using the overloaded operators:

  - Addition: `BigInt c = a + b;`

  - Subtraction: `BigInt c = a - b;`

  - Multiplication: `BigInt c = a * b;`

  - Division: `BigInt c = a / b;`

  - Modulo: `BigInt c = a % b;`

  - Exponentiation: `BigInt c = a ^ b;`

4. Convert `BigInt` to `int` or `string` using the provided cast operators:

  - `int x = (int)a;`

  - `string s = (string)a;`

### Example Usage

```cpp
#include "BigInt.h"

int main() {
   BigInt a("123456789012345678901234567890");
   BigInt b("987654321098765432109876543210");

   cout << "a = " << a << endl;
   cout << "b = " << b << endl;

   BigInt c = a + b;
   cout << "a + b = " << c << endl;

   c = a - b;
   cout << "a - b = " << c << endl;

   c = a * b;
   cout << "a * b = " << c << endl;

   c = a / b;
   cout << "a / b = " << c << endl;

   c = a % b;
   cout << "a % b = " << c << endl;

   return 0;
}
```

### Time Spent

I spent about 6 hours implementing BigInt class
