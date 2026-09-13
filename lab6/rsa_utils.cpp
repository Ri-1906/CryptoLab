#include <cstdlib>
#include "miller_rabin2.cpp"

long long gcd(long long a, long long b) {
    return b == 0 ? a : gcd(b, a % b);
}

// Modular inverse using Extended Euclid
long long modInverse(long long e, long long phi) {
    long long t = 0, newt = 1;
    long long r = phi, newr = e;

    while (newr != 0) {
        long long q = r / newr;
        long long temp = newt;
        newt = t - q * newt;
        t = temp;

        temp = newr;
        newr = r - q * newr;
        r = temp;
    }

    if (t < 0) t += phi;
    return t;
}

// Prime generation USING YOUR MILLER–RABIN
long long generatePrime() {
    while (true) {
        long long p = rand() % 9000 + 1000;
        if (p % 2 == 0) p++;
        if (millerRabin(p))
            return p;
    }
}
