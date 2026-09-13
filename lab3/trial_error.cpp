#include <iostream>
#include <chrono>
#include <iomanip>
using namespace std;
using namespace std::chrono;

int primeNumber(int a) {
    if (a == 0 || a == 1) return -1;

    for (int i = 2; i * i <= a; i++) {
        if (a % i == 0)
            return -1;
    }
    return 1;
}

int main() {
    int a;
    cout << "Enter a number: ";
    cin >> a;

    auto start = high_resolution_clock::now();

    int result = primeNumber(a);

    auto stop = high_resolution_clock::now();

    // Use nanoseconds for higher resolution
    duration<double, nano> time_taken = stop - start;

    cout << fixed << setprecision(6);

    if (result == -1)
        cout << "Not a prime number" << endl;
    else
        cout << "Prime number" << endl;

    cout << "Time taken: " << time_taken.count() << " microseconds" << endl;

    return 0;
}
