
struct selfref {
    selfref* next = nullptr;
};

struct exec {
    selfref mem = selfref{};

    constexpr exec() {
        mem.next = &mem;
    }
};

constexpr exec do_thing() {
    return exec{};
}


constexpr exec ret = do_thing();
constexpr selfref* ptr = ret.mem.next;

