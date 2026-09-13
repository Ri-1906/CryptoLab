#include <iostream>
#include <cstdlib>
using namespace std;

// Fast modular exponentiation
long long power(long long a, long long d, long long n) {
    long long result = 1;
    a %= n;

    while (d > 0) {
        if (d & 1)
            result = (result * a) % n;
        a = (a * a) % n;
        d >>= 1;
    }
    return result;
}

// Simple Miller–Rabin
bool millerRabin(long long n, int k = 5) {
    if (n <= 1) return false;
    if (n <= 3) return true;
    if (n % 2 == 0) return false;

    long long d = n - 1;
    while (d % 2 == 0)
        d /= 2;

    for (int i = 0; i < k; i++) {
        long long a = 2 + rand() % (n - 4);
        long long x = power(a, d, n);

        if (x == 1 || x == n - 1)
            continue;

        long long temp = d;
        bool composite = true;

        while (temp != n - 1) {
            x = (x * x) % n;
            temp *= 2;

            if (x == 1) return false;
            if (x == n - 1) {
                composite = false;
                break;
            }
        }

        if (composite) return false;
    }
    return true;
}
