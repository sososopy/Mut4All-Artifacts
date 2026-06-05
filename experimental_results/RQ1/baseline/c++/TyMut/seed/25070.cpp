
struct bitfield {
    void and_assign() volatile {
        _raw = _and(_raw, 1); // this reads twice from _raw
        //mov     eax, DWORD PTR [rsp-24]
        //mov     eax, DWORD PTR [rsp-24]
        //and     eax, 1
        //mov     DWORD PTR [rsp-24], eax
        _raw = _raw & 1; // this reads once from _raw
        //mov     eax, DWORD PTR [rsp-24]
        //and     eax, 1
        //mov     DWORD PTR [rsp-24], eax
    }

    static unsigned _and(unsigned lhs, unsigned rhs) {
        return lhs & rhs;
    }

    unsigned _raw;
};

void test_device() {
    volatile bitfield tcc;

    tcc.and_assign();
}

