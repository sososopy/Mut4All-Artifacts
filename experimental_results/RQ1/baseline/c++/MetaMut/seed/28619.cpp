template <int N>
struct jj {
    int ii[N ?: 1];
};

int main() {
    jj<2> kk;
}
