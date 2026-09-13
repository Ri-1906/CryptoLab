// ===== COMMON_CRYPTO.H =====
#ifndef COMMON_CRYPTO_H
#define COMMON_CRYPTO_H

#include <bits/stdc++.h>
using namespace std;

using ll = long long;

// Fast modular exponentiation
ll modexp(ll base, ll exp, ll mod) {
    ll res = 1;
    base %= mod;
    while (exp > 0) {
        if (exp & 1) res = (__int128)res * base % mod;
        base = (__int128)base * base % mod;
        exp >>= 1;
    }
    return res;
}

// Miller-Rabin Test
bool millerTest(ll d, ll n) {
    ll a = 2 + rand() % (n - 4);
    ll x = modexp(a, d, n);

    if (x == 1 || x == n - 1)
        return true;

    while (d != n - 1) {
        x = (__int128)x * x % n;
        d *= 2;

        if (x == 1) return false;
        if (x == n - 1) return true;
    }
    return false;
}

bool isPrime(ll n, int k = 5) {
    if (n <= 1 || n == 4) return false;
    if (n <= 3) return true;

    ll d = n - 1;
    while (d % 2 == 0)
        d /= 2;

    for (int i = 0; i < k; i++)
        if (!millerTest(d, n))
            return false;

    return true;
}

// Generate random 4-digit prime
ll generatePrime() {
    while (true) {
        ll num = 1000 + rand() % 9000;
        if (num % 2 == 0) num++;
        while (!isPrime(num)) num += 2;
        return num;
    }
}

// GCD
ll gcd(ll a, ll b) {
    return b == 0 ? a : gcd(b, a % b);
}

// Modular inverse (Extended Euclid)
ll modInverse(ll a, ll m) {
    ll m0 = m, t, q;
    ll x0 = 0, x1 = 1;

    if (m == 1) return 0;

    while (a > 1) {
        q = a / m;
        t = m;
        m = a % m, a = t;
        t = x0;
        x0 = x1 - q * x0;
        x1 = t;
    }

    if (x1 < 0) x1 += m0;
    return x1;
}

#endif