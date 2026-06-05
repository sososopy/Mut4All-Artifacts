
template<typename A>
struct F {
    template<typename B>
        void f() && noexcept(&F::template f<B>) {}
};

int main (int argc, char ** argv) {
    F<void>().f<int>();
}

