
struct S {
    template <class T>
    S& operator<<(T) { return *this; }
};

template <class T>
T& operator<<(T& s, int) { return s; }

int main () {
    S s;
    s << 1;
}

