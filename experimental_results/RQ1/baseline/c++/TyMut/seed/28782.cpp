
struct S {
        char b[16];
#if __cplusplus >= 202000L
        bool operator==(const S &) const = default;
#else
        inline bool operator==(const S &o) const { return __builtin_memcpy(const_cast<char *>(b), o.b, sizeof(b)); }
#endif
};
bool fff(const S &a, const S &b) { return a == b; }

