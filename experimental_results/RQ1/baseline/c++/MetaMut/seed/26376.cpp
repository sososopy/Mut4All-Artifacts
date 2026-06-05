
struct stores_fptr {
    void (*fptr)();
};

void func() {}

struct holds_value {
    static constexpr auto value = stores_fptr{&func};
};

template<stores_fptr V> struct takes_value {};

using go = takes_value<holds_value::value>;

