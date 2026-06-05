

struct Struct {
    Struct() = default;
    Struct(Struct&&);
};

Struct question10(bool b) {
    if (b) {
        Struct s{};
        return s;
    } else {
        return {};
    }
}

