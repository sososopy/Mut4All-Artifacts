
template<typename T>
concept bool FC() { return true; }
int main() {
    auto fc = &FC<int>;
    fc();
}

