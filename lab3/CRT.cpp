#include <iostream>
#include <vector>
#include <chrono>
#include <iomanip>
using namespace std;
using namespace std::chrono;

// Extended Euclidean Algorithm
long long gcdExtended(long long a, long long b, long long &x, long long &y) {
    if (b == 0) {
        x = 1;
        y = 0;
        return a;
    }
    long long x1, y1;
    long long gcd = gcdExtended(b, a % b, x1, y1);
    x = y1;
    y = x1 - y1 * (a / b);
    return gcd;
}

// Modular inverse of a mod m
long long modInverse(long long a, long long m) {
    long long x, y;
    long long g = gcdExtended(a, m, x, y);
    return (x % m + m) % m;
}

int main() {
    int n;
    cout << "Enter number of congruences: ";
    cin >> n;

    vector<long long> a(n), m(n);

    cout << "Enter remainders:\n";
    for (int i = 0; i < n; i++)
        cin >> a[i];

    cout << "Enter moduli:\n";
    for (int i = 0; i < n; i++)
        cin >> m[i];

    auto start = high_resolution_clock::now();

    // Step 1: Compute product of all moduli
    long long M = 1;
    for (int i = 0; i < n; i++)
        M *= m[i];

    // Step 2: Apply CRT formula
    long long result = 0;
    for (int i = 0; i < n; i++) {
        long long Mi = M / m[i];
        long long inv = modInverse(Mi, m[i]);
        result = (result + a[i] * Mi % M * inv % M) % M;
    }

    auto stop = high_resolution_clock::now();
    duration<double, micro> time_taken = stop - start;

    cout << fixed << setprecision(6);
    cout << "\nSolution x = " << result << endl;
    cout << "Time taken: " << time_taken.count() << " microseconds" << endl;

    return 0;
}
