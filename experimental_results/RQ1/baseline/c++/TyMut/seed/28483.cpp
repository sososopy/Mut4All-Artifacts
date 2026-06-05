

template <typename X>
bool before(bool ascending, X & a, X * b) {
    return ascending ? a.value < (*b).value : a.value > (*b).value;
}

