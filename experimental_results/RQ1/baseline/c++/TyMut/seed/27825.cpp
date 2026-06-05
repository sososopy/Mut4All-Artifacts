
void foo () {
    using T = int&;
    int i{};
    T{i};
}

