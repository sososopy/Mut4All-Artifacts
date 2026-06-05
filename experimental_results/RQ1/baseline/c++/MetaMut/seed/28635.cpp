
int bar(unsigned short x, int* cache = nullptr) {
    if (!cache) {
        int cache[1 << (sizeof(x) * 8)] = {};
        return bar(x, cache);
    }
    if (cache[x] != 0) {
        return cache[x];
    }
    if (x == 0) {
        cache[x] = 1;
    } else {
        cache[x] = (bar(x - 1, cache) + bar(x / 2, cache)) % 1000000007;
    }
    return cache[x];
}

int bar2(unsigned short x, int* cache = nullptr) {
    if (!cache) {
        return [&] {
            int cache[1 << (sizeof(x) * 8)] = {};
            return bar2(x, cache);
        }();
    }
    if (cache[x] != 0) {
        return cache[x];
    }
    if (x == 0) {
        cache[x] = 1;
    } else {
        cache[x] = (bar2(x - 1, cache) + bar2(x / 2, cache)) % 1000000007;
    }
    return cache[x];
}

#include <cstdio>
#include <cstdlib>

int main() {
    printf("%i\n", bar(65535));
    system("grep Vm /proc/$PPID/status");
}
