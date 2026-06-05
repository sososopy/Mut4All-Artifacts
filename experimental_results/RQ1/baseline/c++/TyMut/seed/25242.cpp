
void sort_of_sieve(bool primes[], int n) {
    for (int i = 2; i <= n; i++) {

        primes[i] = true;
    }
}

void factorize(int n, int OUT[]) {
    bool primes[n];

    int factors = 0;
    sort_of_sieve(primes, n);


