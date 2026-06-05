
struct StringWrapper {
    const char* Value;
};

template <typename T>
void f() {
    [](auto) {
        StringWrapper{__PRETTY_FUNCTION__};
    };
}

int main() {
    f<int>();
}

