
export module mod;

export constexpr void f() {
    enum { a };
    a;
}


import mod;

int main() {
    f();
}

