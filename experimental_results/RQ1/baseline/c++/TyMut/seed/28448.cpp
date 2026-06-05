
struct E {
    enum class type : unsigned char { UNKNOWN };
};

struct T {
    using enum E::type;
};

template <int N>
struct U : T
{
    int ice(int x) {
        return static_cast<int>(UNKNOWN);
    }
};

int main() {

    U<3>().ice(3);
}
