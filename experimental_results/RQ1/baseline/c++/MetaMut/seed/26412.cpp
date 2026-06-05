
template <class T>
[[gnu::abi_tag("ABI")]] inline int value = 0;

int get() {
    return value<int>;
}

