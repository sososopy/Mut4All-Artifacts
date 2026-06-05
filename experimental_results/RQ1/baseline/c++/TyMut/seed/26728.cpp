
template <int N>
struct I0ATerm {
        static const double value = I0ATerm<N-1>::value * (2.*N-1.) * (2.*N-1.) / (8. * N);
};

template <>
struct I0ATerm<0> { // 1/sqrt(2*PI);
        static const double value = 0.398942280401432677939946059934381868475858631164934657665925;
};

int main(int argc, char **argv) {
        I0ATerm<1>::value;
}


