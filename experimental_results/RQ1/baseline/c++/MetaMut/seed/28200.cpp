
    struct probe {
        template<typename T>
        operator T const&& ();
    };

    void frob(int const&) {}

    int main() {
        frob(probe()); // don't work with GCC 9.1
    }

