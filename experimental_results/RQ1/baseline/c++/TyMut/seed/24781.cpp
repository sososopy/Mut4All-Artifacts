struct nested_anon_bitfield_init_t {
    union {
        struct {
            int inner : 32 { 42 };
        };
        int outer;
    };
};

int main() {
    return nested_anon_bitfield_init_t{}.inner;
}